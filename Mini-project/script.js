
const graph = {
    "A": { "B": 5, "C": 10 },
    "B": { "A": 5, "D": 3 },
    "C": { "A": 10, "D": 1 },
    "D": { "B": 3, "C": 1, "E": 7 },
    "E": { "D": 7 }
};


function dijkstra(start, end) {
    let distances = {};
    let previous = {};
    let nodes = [];

   
    for (let node in graph) {
        distances[node] = Infinity;
        previous[node] = null;
        nodes.push(node);
    }

    distances[start] = 0;

    while (nodes.length > 0) {
       
        nodes.sort((a, b) => distances[a] - distances[b]);

        let closest = nodes.shift();

        if (closest === end) break;
        if (distances[closest] === Infinity) break;

        for (let neighbor in graph[closest]) {
            let newDist = distances[closest] + graph[closest][neighbor];

            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = closest;
            }
        }
    }

    
    let path = [];
    let current = end;

    while (current) {
        path.unshift(current);
        current = previous[current];
    }

    return {
        distance: distances[end],
        path: path
    };
}


function bookRide() {
    const pickup = document.getElementById("pickup").value.trim().toUpperCase();
    const drop = document.getElementById("drop").value.trim().toUpperCase();

    
    if (!pickup || !drop) {
        alert("Please enter both locations!");
        return;
    }

    if (!graph[pickup] || !graph[drop]) {
        alert("Invalid locations! Use only: A, B, C, D, E");
        return;
    }

   
    document.getElementById("loading").classList.remove("hidden");
    document.getElementById("result").classList.add("hidden");

    
    setTimeout(() => {

        const result = dijkstra(pickup, drop);

        if (!result.distance || result.distance === Infinity) {
            alert("No route found!");
            return;
        }

        
        const fare = result.distance * 10;

       
        const drivers = ["Rahul", "Amit", "Suresh", "Priya"];
        const driver = drivers[Math.floor(Math.random() * drivers.length)];

        
        document.getElementById("route").innerText =
            `Route: ${result.path.join(" → ")}`;

        document.getElementById("distance").innerText =
            `Distance: ${result.distance} km`;

        document.getElementById("fare").innerText =
            `Fare: ₹${fare}`;

        document.getElementById("driver").innerText =
            `Driver Assigned: ${driver}`;

        
        document.getElementById("loading").classList.add("hidden");
        document.getElementById("result").classList.remove("hidden");

    }, 1500);
}