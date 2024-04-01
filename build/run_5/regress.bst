=== BEST-OF-RUN ===
              generation: 4
                   nodes: 27
                   depth: 6
                    hits: 907
TOP INDIVIDUAL:



-- #1 --
                    hits: 907
             raw fitness: 907.0000
    standardized fitness: 907.0000
        adjusted fitness: 0.9989
TREE:
 (/ (* (/ (log extent)
          (exp major))
       (log (exp eccentricity)))
    (- (exp (+ perimeter minor))
       (- (* minor extent)
          (- (log (- convex convex))
             (/ eccentricity minor)))))
TREE-equ:
y = (((log(extent) / exp(major)) * log(exp(eccentricity))) / (exp((perimeter + minor)) - ((minor * extent) - (log((convex - convex)) - (eccentricity / minor)))))
