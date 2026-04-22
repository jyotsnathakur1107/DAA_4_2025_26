/*
 * ╔══════════════════════════════════════════════════╗
 * ║         CAB BOOKING SIMULATION (Uber-Like)       ║
 * ║   OOP + Dijkstra's Shortest Path + Ride History  ║
 * ╚══════════════════════════════════════════════════╝
 *
 * Compile: g++ -o cab_booking cab_booking.cpp
 * Run (Windows): cab_booking.exe
 * Run (Mac/Linux): ./cab_booking
 *
 * Compatible with C++11 and above — no structured bindings.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;


const int    INF          = numeric_limits<int>::max();
const double RATE_PER_KM  = 12.0;
const double BASE_FARE    = 30.0;


enum DriverStatus { AVAILABLE, ON_TRIP };
enum RideStatus   { COMPLETED, CANCELLED };


class CityGraph {
public:
    map<string, vector<pair<string,int>>> adj;

    void addEdge(const string& u, const string& v, int dist) {
        adj[u].push_back(make_pair(v, dist));
        adj[v].push_back(make_pair(u, dist));
    }

    
    pair<int, vector<string>> shortestPath(const string& src,
                                           const string& dst) {
        map<string,int>    dist;
        map<string,string> prev;

      
        for (map<string,vector<pair<string,int>>>::iterator it = adj.begin();
             it != adj.end(); ++it)
            dist[it->first] = INF;

        dist[src] = 0;

        priority_queue<pair<int,string>,
                       vector<pair<int,string>>,
                       greater<pair<int,string>>> pq;
        pq.push(make_pair(0, src));

        while (!pq.empty()) {
            int    d = pq.top().first;
            string u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            vector<pair<string,int>>& nbrs = adj[u];
            for (int i = 0; i < (int)nbrs.size(); i++) {
                string v = nbrs[i].first;
                int    w = nbrs[i].second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        if (dist[dst] == INF) return make_pair(-1, vector<string>());

        vector<string> path;
        for (string cur = dst; cur != src; cur = prev[cur])
            path.push_back(cur);
        path.push_back(src);
        reverse(path.begin(), path.end());

        return make_pair(dist[dst], path);
    }

    void printMap() const {
        cout << "\n  City Road Network:\n";
        cout << "  " << string(40, '-') << "\n";
        set<pair<string,string>> printed;
        for (map<string,vector<pair<string,int>>>::const_iterator it = adj.begin();
             it != adj.end(); ++it) {
            const string& u = it->first;
            const vector<pair<string,int>>& nbrs = it->second;
            for (int i = 0; i < (int)nbrs.size(); i++) {
                const string& v = nbrs[i].first;
                int           d = nbrs[i].second;
                string lo = min(u,v), hi = max(u,v);
                if (!printed.count(make_pair(lo,hi))) {
                    cout << "   " << u << " <-> " << v
                         << "  (" << d << " km)\n";
                    printed.insert(make_pair(lo,hi));
                }
            }
        }
    }
};


struct Ride {
    int            rideId;
    string         passengerName;
    string         driverName;
    string         from, to;
    int            distanceKm;
    double         fare;
    RideStatus     status;
    vector<string> route;

    void print() const {
        string statusStr = (status == COMPLETED) ? "[Completed]" : "[Cancelled]";
        cout << "  +-- Ride #" << rideId << " --------------------------\n";
        cout << "  |  Passenger : " << passengerName << "\n";
        cout << "  |  Driver    : " << driverName    << "\n";
        cout << "  |  Route     : ";
        for (int i = 0; i < (int)route.size(); i++) {
            cout << route[i];
            if (i + 1 < (int)route.size()) cout << " -> ";
        }
        cout << "\n";
        cout << "  |  Distance  : " << distanceKm << " km\n";
        cout << "  |  Fare      : Rs." << fixed << setprecision(2) << fare << "\n";
        cout << "  |  Status    : " << statusStr << "\n";
        cout << "  +------------------------------------------\n";
    }
};


class Driver {
public:
    int          id;
    string       name;
    string       location;
    DriverStatus status;
    double       totalEarnings;
    int          totalRides;

    Driver(int id_, const string& n, const string& loc)
        : id(id_), name(n), location(loc),
          status(AVAILABLE), totalEarnings(0.0), totalRides(0) {}

    void assignRide(const string& dest) {
        status   = ON_TRIP;
        location = dest;
    }

    void completeRide(double fare) {
        status         = AVAILABLE;
        totalEarnings += fare;
        totalRides++;
    }

    void print() const {
        string s = (status == AVAILABLE) ? "[Available]" : "[On Trip  ]";
        cout << "   [D" << id << "] "
             << left << setw(10) << name
             << "  Loc: " << setw(12) << location
             << "  " << s
             << "  Rides: "    << totalRides
             << "  Earned: Rs." << fixed << setprecision(2) << totalEarnings
             << "\n";
    }
};


class Passenger {
public:
    int         id;
    string      name;
    vector<int> rideHistory;

    Passenger(int id_, const string& n) : id(id_), name(n) {}

    void addRide(int rid) { rideHistory.push_back(rid); }
};

// ─────────────────────────────────────────────
//  BOOKING SYSTEM
// ─────────────────────────────────────────────
class BookingSystem {
    CityGraph         graph;
    vector<Driver>    drivers;
    vector<Passenger> passengers;
    vector<Ride>      rides;
    int               nextRideId;

    double calcFare(int km) const {
        return BASE_FARE + km * RATE_PER_KM;
    }
    
    Driver* findNearestDriver(const string& from) {
        Driver* best     = NULL;
        int     bestDist = INF;

        for (int i = 0; i < (int)drivers.size(); i++) {
            Driver& d = drivers[i];
            if (d.status != AVAILABLE) continue;
            if (d.location == from)   return &d;

            pair<int,vector<string>> res =
                graph.shortestPath(d.location, from);
            int dist = res.first;
            if (dist != -1 && dist < bestDist) {
                bestDist = dist;
                best     = &d;
            }
        }
        return best;
    }

    Passenger* findPassenger(const string& name) {
        for (int i = 0; i < (int)passengers.size(); i++)
            if (passengers[i].name == name)
                return &passengers[i];
        return NULL;
    }

public:
    BookingSystem() : nextRideId(1) {}


    void buildCity() {
        graph.addEdge("Airport",    "Mall",       8);
        graph.addEdge("Airport",    "Station",   12);
        graph.addEdge("Mall",       "Park",       5);
        graph.addEdge("Mall",       "Hospital",   7);
        graph.addEdge("Station",    "Park",       6);
        graph.addEdge("Station",    "Downtown",  10);
        graph.addEdge("Park",       "Downtown",   4);
        graph.addEdge("Park",       "University", 9);
        graph.addEdge("Hospital",   "University", 6);
        graph.addEdge("Downtown",   "University", 8);
        graph.addEdge("Downtown",   "Airport",   15);
    }

    void addDriver(const string& name, const string& loc) {
        int id = (int)drivers.size() + 1;
        drivers.push_back(Driver(id, name, loc));
    }

    void addPassenger(const string& name) {
        int id = (int)passengers.size() + 1;
        passengers.push_back(Passenger(id, name));
    }

    
    bool bookRide(const string& passengerName,
                  const string& from,
                  const string& to) {
        cout << "\n  >> Booking ride for " << passengerName
             << " : " << from << " -> " << to << "\n";

        Passenger* p = findPassenger(passengerName);
        if (!p) {
            cout << "  ERROR: Passenger not found.\n";
            return false;
        }

        pair<int,vector<string>> res = graph.shortestPath(from, to);
        int            dist  = res.first;
        vector<string> route = res.second;
        

        if (dist == -1) {
            cout << "  ERROR: No route between "
                 << from << " and " << to << ".\n";
            return false;
        }

        Driver* d = findNearestDriver(from);
        if (!d) {
            cout << "  ERROR: No drivers available.\n";
            return false;
        }

        double fare = calcFare(dist);

        Ride ride;
        ride.rideId        = nextRideId++;
        ride.passengerName = passengerName;
        ride.driverName    = d->name;
        ride.from          = from;
        ride.to            = to;
        ride.distanceKm    = dist;
        ride.fare          = fare;
        ride.status        = COMPLETED;
        ride.route         = route;

        rides.push_back(ride);
        p->addRide(ride.rideId);

        string driverPrevLoc = d->location;
        d->assignRide(to);
        d->completeRide(fare);

        cout << "  " << string(43, '-') << "\n";
        cout << "  RIDE CONFIRMED!\n";
        cout << "  Driver   : " << d->name
             << " (from " << driverPrevLoc << ")\n";
        cout << "  Path     : ";
        for (int i = 0; i < (int)route.size(); i++) {
            cout << route[i];
            if (i + 1 < (int)route.size()) cout << " -> ";
        }
        cout << "\n";
        cout << "  Distance : " << dist << " km\n";
        cout << "  Fare     : Rs." << fixed << setprecision(2) << fare << "\n";
        cout << "  " << string(43, '-') << "\n";
        return true;
    }

    
    void showDrivers() const {
        cout << "\n  DRIVER STATUS:\n";
        cout << "  " << string(65, '-') << "\n";
        for (int i = 0; i < (int)drivers.size(); i++)
            drivers[i].print();
    }

    void showRideHistory() const {
        cout << "\n  ALL RIDES:\n";
        if (rides.empty()) { cout << "  (No rides yet)\n"; return; }
        for (int i = 0; i < (int)rides.size(); i++)
            rides[i].print();
    }

    void showPassengerHistory(const string& name) const {
        const Passenger* p = NULL;
        for (int i = 0; i < (int)passengers.size(); i++)
            if (passengers[i].name == name) { p = &passengers[i]; break; }
        if (!p) { cout << "  Passenger not found.\n"; return; }

        cout << "\n  RIDE HISTORY FOR: " << name << "\n";
        if (p->rideHistory.empty()) { cout << "  (No rides)\n"; return; }
        for (int i = 0; i < (int)p->rideHistory.size(); i++) {
            int rid = p->rideHistory[i];
            for (int j = 0; j < (int)rides.size(); j++)
                if (rides[j].rideId == rid) { rides[j].print(); break; }
        }
    }

    void showMap() const { graph.printMap(); }

    void showPassengers() const {
        cout << "\n  PASSENGERS:\n";
        cout << "  " << string(40, '-') << "\n";
        for (int i = 0; i < (int)passengers.size(); i++)
            cout << "   [P" << passengers[i].id << "] "
                 << passengers[i].name
                 << "  | Rides taken: "
                 << passengers[i].rideHistory.size() << "\n";
    }
};


void banner() {
    cout << "\n";
    cout << "  ==========================================\n";
    cout << "       CAB BOOKING SIMULATION (Uber-Like)\n";
    cout << "        OOP  |  Dijkstra  |  Ride History\n";
    cout << "  ==========================================\n";
}

void menu() {
    cout << "\n";
    cout << "  +----------- MENU ----------------+\n";
    cout << "  |  1. Book a Ride                 |\n";
    cout << "  |  2. View All Drivers            |\n";
    cout << "  |  3. View Full Ride History      |\n";
    cout << "  |  4. View Passenger History      |\n";
    cout << "  |  5. Show City Map               |\n";
    cout << "  |  6. Show Passengers             |\n";
    cout << "  |  0. Exit                        |\n";
    cout << "  +---------------------------------+\n";
    cout << "  Choice: ";
}


int main() {
    BookingSystem sys;

    sys.buildCity();

    sys.addDriver("Arjun",  "Airport");
    sys.addDriver("Priya",  "Downtown");
    sys.addDriver("Rahul",  "Station");
    sys.addDriver("Sneha",  "Mall");

    sys.addPassenger("Jyotsna Thakur");
    sys.addPassenger("Nakul");
    sys.addPassenger("Rajbir");

    banner();

    int choice;
    while (true) {
        menu();
        cin >> choice;
        cin.ignore();

        if (choice == 0) {
            cout << "\n  Goodbye! Thanks for using CabSim.\n\n";
            break;
        }

        switch (choice) {
        case 1: {
            string pname, from, to;
            sys.showPassengers();
            cout << "\n  Enter passenger name : ";
            getline(cin, pname);
            sys.showMap();
            cout << "\n  Pickup location  : ";
            getline(cin, from);
            cout << "  Drop location    : ";
            getline(cin, to);
            sys.bookRide(pname, from, to);
            break;
        }
        case 2: sys.showDrivers();   break;
        case 3: sys.showRideHistory(); break;
        case 4: {
            string pname;
            sys.showPassengers();
            cout << "\n  Enter passenger name : ";
            getline(cin, pname);
            sys.showPassengerHistory(pname);
            break;
        }
        case 5: sys.showMap();       break;
        case 6: sys.showPassengers(); break;
        default:
            cout << "  Invalid choice. Try again.\n";
        }
    }

    return 0;
}
