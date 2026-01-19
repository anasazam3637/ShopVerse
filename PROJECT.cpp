#include <iostream>
using namespace std;

  // HELPER FUNCTIONS 

int getStrLen(string s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

string toLower(string s) {
    for (int i = 0; i < getStrLen(s); i++)
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] += 32;
    return s;
}

int strEqual(string a, string b) {
    return toLower(a) == toLower(b);
}

//  DATA MODELS 

class User {
public:
    string username, password, city;
    int couponUsed;
};

class Product {
public:
    int id;
    string name, size, color;
    float price;
    int quantity;
};

Product products[50];
int pCount = 0;
float revenue = 0;

//   PRELOAD 

void preload() {
    products[pCount++] = {101, "TShirt", "M", "Red", 500, 10};
    products[pCount++] = {101, "TShirt", "L", "Red", 500, 8};
    products[pCount++] = {102, "Jeans", "L", "Blue", 1200, 15};
    products[pCount++] = {103, "Jacket", "XL", "Black", 3500, 5};
}

//  CART (LINKED LIST) 

class CartNode {
public:
    Product p;
    int qty;
    CartNode* next;
};

class Cart {
public:
    CartNode* head;
    Cart() { head = NULL; }

    void add(Product pr) {
        CartNode* temp = head;
        while (temp) {
            if (temp->p.id == pr.id &&
                strEqual(temp->p.size, pr.size) &&
                strEqual(temp->p.color, pr.color)) {
                temp->qty++;
                cout << "Quantity increased in cart\n";
                return;
            }
            temp = temp->next;
        }
        CartNode* n = new CartNode();
        n->p = pr; n->qty = 1; n->next = head; head = n;
        cout << "Added to cart\n";
    }

    void removeItem() {
        if (!head) { cout << "Cart empty\n"; return; }
        int id; cout << "Enter Product ID to remove: "; cin >> id;
        CartNode *cur = head, *prev = NULL;
        while (cur) {
            if (cur->p.id == id) {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                delete cur;
                cout << "Item removed\n";
                return;
            }
            prev = cur; cur = cur->next;
        }
        cout << "Item not found\n";
    }

    float total() {
        float t = 0;
        CartNode* temp = head;
        while (temp) {
            t += temp->p.price * temp->qty;
            temp = temp->next;
        }
        return t;
    }

    void show() {
        CartNode* t = head;
        if (!t) { cout << "Cart empty\n"; return; }
        while (t) {
            cout << t->p.name << " (" << t->p.size << "," << t->p.color
                 << ") x" << t->qty << " = Rs."
                 << t->p.price * t->qty << endl;
            t = t->next;
        }
        cout << "Total: Rs." << total() << endl;
    }

    void clear() {
        while (head) {
            CartNode* t = head;
            head = head->next;
            delete t;
        }
    }
};

//  ORDER STACK 

class OrderStack {
    float bills[10];
    int top;
public:
    OrderStack() { top = -1; }
    void push(float b) { if (top < 9) bills[++top] = b; }
    void show() {
        if (top == -1) cout << "No history\n";
        for (int i = top; i >= 0; i--)
            cout << "Paid Rs." << bills[i] << endl;
    }
};

//  ORDER QUEUE 

class OrderQueue {
    string status[10];
    int rear;
public:
    OrderQueue() { rear = -1; }
    void add(string s) { if (rear < 9) status[++rear] = s; }
    void track() {
        if (rear == -1) cout << "No active orders\n";
        else cout << "Order Status: " << status[rear] << endl;
    }
};

//  MAIN 

int main() {
    preload();

    User users[10];
    int uCount = 0;

    Cart cart;
    OrderStack history;
    OrderQueue queue;

    int mainCh;
    do {
        cout << "\n1.Register \n2.Login \n3.Admin \n4.Exit: " << endl; 
        cin >> mainCh;

        // REGISTER 
        if (mainCh == 1) {
            string u; cout << "Username: "; cin >> u;
            for (int i = 0; i < uCount; i++)
                if (strEqual(users[i].username, u)) {
                    cout << "Username exists\n"; goto skip;
                }
            users[uCount].username = u;
            cout << "Password: "; cin >> users[uCount].password;
            cout << "City: "; cin >> users[uCount].city;
            users[uCount].couponUsed = 0;
            uCount++;
            cout << "Registered\n";
        }
        skip:

        // USER LOGIN 
        if (mainCh == 2) {
            string u,p;
            cout << "UserName: "; cin >> u;
            cout << "Password: "; cin >> p;
            int idx = -1;
            for (int i = 0; i < uCount; i++)
                if (strEqual(users[i].username,u) && users[i].password==p)
                    idx = i;

            if (idx == -1) { cout << "Invalid login\n"; continue; }

            int ch;
            do {
                cout << "\n1.Catalog \n2.Buy \n3.Cart \n4.Remove \n5.Checkout \n6.Track \n7.History \n8.Logout: ";
                cin >> ch;

                if (ch == 1)
                    for (int i = 0; i < pCount; i++)
                        cout << products[i].id << " "
                             << products[i].name << " "
                             << products[i].size << " "
                             << products[i].color << " Rs."
                             << products[i].price << " Qty:"
                             << products[i].quantity << endl;

                else if (ch == 2) {
                    int id; string s,c;
                    cout << "ID: "; cin >> id;
                    cout << "Size: "; cin >> s;
                    cout << "Color: "; cin >> c;
                    for (int i = 0; i < pCount; i++)
                        if (products[i].id==id &&
                            strEqual(products[i].size,s) &&
                            strEqual(products[i].color,c) &&
                            products[i].quantity>0) {
                            cart.add(products[i]);
                            products[i].quantity--;
                            break;
                        }
                }

                else if (ch == 3) cart.show();
                else if (ch == 4) cart.removeItem();

                else if (ch == 5) {
                    float total = cart.total();
                    if (total == 0) { cout << "Cart empty\n"; continue; }

                    cout << "Deliver to " << users[idx].city << " (1-change 0-ok): ";
                    int ed; cin >> ed;
                    if (ed==1) cin >> users[idx].city;

                    if (!users[idx].couponUsed) {
                        string cp; cout << "Coupon: "; cin >> cp;
                        if (strEqual(cp,"SAVE10")) {
                            total *= 0.9;
                            users[idx].couponUsed = 1;
                        }
                    }

                    cout << "Confirm order? (1=yes): ";
                    int cf; cin >> cf;
                    if (cf==1) {
                        revenue += total;
                        history.push(total);
                        queue.add("Processing");
                        cart.clear();
                        cout << "Order placed Rs." << total << endl;
                    }
                }

                else if (ch == 6) queue.track();
                else if (ch == 7) history.show();
                else if (ch == 8) cart.clear();

            } while (ch != 8);
        }

        // ADMIN 
        if (mainCh == 3) {
            string a,p;
            cout << "Admin: "; cin >> a;
            cout << "Pass: "; cin >> p;
            if (!strEqual(a,"admin") || p!="admin123") continue;

            int aCh;
            do {
                cout << "\n1.Add Product 2.Restock 3.Revenue 4.Stock 5.Logout: ";
                cin >> aCh;

                if (aCh==1) {
                    products[pCount++] = {};
                    cout << "ID:"; cin >> products[pCount-1].id;
                    cout << "Name:"; cin >> products[pCount-1].name;
                    cout << "Size:"; cin >> products[pCount-1].size;
                    cout << "Color:"; cin >> products[pCount-1].color;
                    cout << "Price:"; cin >> products[pCount-1].price;
                    cout << "Qty:"; cin >> products[pCount-1].quantity;
                }
                else if (aCh==2) {
                    int id,q;
                    cout << "ID:"; cin >> id;
                    cout << "Add Qty:"; cin >> q;
                    for(int i=0;i<pCount;i++)
                        if(products[i].id==id) products[i].quantity+=q;
                }
                else if (aCh==3) cout << "Revenue Rs." << revenue << endl;
                else if (aCh==4)
                    for(int i=0;i<pCount;i++)
                        cout << products[i].name << " " << products[i].quantity << endl;

            } while (aCh != 5);
        }

    } while (mainCh != 4);

    return 0;
}
