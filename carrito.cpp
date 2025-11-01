#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <limits>
#include <iomanip>
using namespace std;

// ===== Producto =====
struct Producto {
    string nombre;
    double precio;
    int stock;
};

// ===== ItemCarrito =====
struct ItemCarrito {
    Producto producto;
    int cantidad;
};

// ===== Carrito =====
class CarritoCompras {
    vector<ItemCarrito> items_;
public:
    void agregar(const Producto& p, int cantidad) {
        items_.push_back({p, cantidad});
        cout << "✅ Producto agregado\n";
    }

    bool eliminar(const string& nombre) {
        auto it = remove_if(items_.begin(), items_.end(), [&](auto& i){
            return i.producto.nombre == nombre;
        });
        if (it == items_.end()) return false;
        items_.erase(it, items_.end());
        return true;
    }

    double total() const {
        double t = 0;
        for (auto& i : items_) t += i.producto.precio * i.cantidad;
        return t;
    }

    bool vacio() const { return items_.empty(); }

    const vector<ItemCarrito>& items() const { return items_; }

    void limpiar() { items_.clear(); }

    void mostrar() const {
        if (items_.empty()) {
            cout << "🛒 Carrito vacío\n";
            return;
        }
        cout << "\n--- Carrito ---\n";
        for (auto& i : items_) {
            cout << i.producto.nombre << " x" << i.cantidad 
                 << " = $" << i.producto.precio * i.cantidad << "\n";
        }
        cout << "Total = $" << total() << "\n";
    }
};

// ===== Usuario =====
struct Compra {
    vector<ItemCarrito> items;
    double total;
    string fecha;
};

class Usuario {
    string nombre_;
    vector<Compra> historial_;
public:
    Usuario(string n): nombre_(n) {}

    void confirmarCompra(CarritoCompras& c) {
        if (c.vacio()) {
            cout << "⚠️ Carrito vacío\n";
            return;
        }
        time_t now = time(nullptr);
        historial_.push_back({c.items(), c.total(), string(ctime(&now))});
        cout << "✅ Compra confirmada por $" << c.total() << "\n";
        c.limpiar();
    }

    void mostrarHistorial() const {
        cout << "\n--- Historial de Compras ---\n";
        if (historial_.empty()) {
            cout << "Sin compras previas\n";
            return;
        }
        for (auto& h : historial_) {
            cout << h.fecha;
            cout << "Total: $" << h.total << "\n\n";
        }
    }
};

// ===== Catálogo =====
class Catalogo {
    vector<Producto> p = {
        {"Pan", 1500, 50},
        {"Leche", 3800, 30},
        {"Huevos (30u)", 16500, 10},
        {"Queso", 7200, 20}
    };
public:
    const Producto* buscar(const string& n) const {
        for (auto& i : p) if (i.nombre == n) return &i;
        return nullptr;
    }
    void mostrar() const {
        cout << "\n📦 Catálogo:\n";
        for (auto& i : p)
            cout << i.nombre << " - $" << i.precio << " (stock " << i.stock << ")\n";
    }
};

// ===== MAIN =====
int main() {
    Usuario u("Cris");
    Catalogo cat;
    CarritoCompras carrito;
    int op;

    do {
        cout << "\n===== MENU =====\n"
             << "1) Ver catálogo\n"
             << "2) Agregar al carrito\n"
             << "3) Eliminar del carrito\n"
             << "4) Ver carrito\n"
             << "5) Confirmar compra\n"
             << "6) Ver historial\n"
             << "0) Salir\n"
             << "Opción: ";
        cin >> op;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (op == 1) cat.mostrar();
        else if (op == 2) {
            string n; int c;
            cout << "Producto: "; getline(cin,n);
            auto p = cat.buscar(n);
            if (!p) cout << "❌ No existe\n";
            else { cout << "Cantidad: "; cin >> c; carrito.agregar(*p,c); }
        }
        else if (op == 3) {
            string n; 
            cout << "Eliminar: "; getline(cin,n);
            cout << (carrito.eliminar(n) ? "✅ Eliminado\n":"❌ No estaba\n");
        }
        else if (op == 4) carrito.mostrar();
        else if (op == 5) u.confirmarCompra(carrito);
        else if (op == 6) u.mostrarHistorial();

    } while(op != 0);

    cout << "👋 Gracias por usar la tienda\n";
    return 0;
}
