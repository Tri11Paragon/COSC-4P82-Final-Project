=== BEST-OF-RUN ===
              generation: 4
                   nodes: 34
                   depth: 8
                    hits: 829
TOP INDIVIDUAL:



-- #1 --
                    hits: 829
             raw fitness: 829.0000
    standardized fitness: 829.0000
        adjusted fitness: 0.9988
TREE:
 (log (/ (/ (* (exp major)
               (+ (- minor 0.24886)
                  (log area)))
            (exp perimeter))
         (- (/ (+ convex convex)
               (/ perimeter convex))
            (- (+ minor extent)
               (/ (* perimeter
                     (exp (+ perimeter eccentricity))) convex)))))
TREE-equ:
y = log((((exp(major) * ((minor - 0.24886) + log(area))) / exp(perimeter)) / (((convex + convex) / (perimeter / convex)) - ((minor + extent) - ((perimeter * exp((perimeter + eccentricity))) / convex)))))
