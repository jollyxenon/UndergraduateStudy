set COMPONENTS;
set PRODUCTS;

param cost {COMPONENTS};
param octane {COMPONENTS};
param sulfur {COMPONENTS};
param availability {COMPONENTS};

param demand {PRODUCTS};
param min_octane {PRODUCTS};
param max_sulfur {PRODUCTS};

var x {COMPONENTS, PRODUCTS} >= 0;

minimize TotalCost:
    sum {c in COMPONENTS, p in PRODUCTS} cost[c] * x[c, p];

subject to ProductionDemand {p in PRODUCTS}:
    sum {c in COMPONENTS} x[c, p] = demand[p];

subject to ComponentAvailability {c in COMPONENTS}:
    sum {p in PRODUCTS} x[c, p] <= availability[c];

subject to OctaneRequirement {p in PRODUCTS}:
    sum {c in COMPONENTS} octane[c] * x[c, p] >= min_octane[p] * demand[p];

subject to SulfurRequirement {p in PRODUCTS}:
    sum {c in COMPONENTS} sulfur[c] * x[c, p] <= max_sulfur[p] * demand[p];