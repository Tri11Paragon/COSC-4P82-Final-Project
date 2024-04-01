=== BEST-OF-RUN ===
              generation: 13
                   nodes: 21
                   depth: 8
                    hits: 905
TOP INDIVIDUAL:



-- #1 --
                    hits: 905
             raw fitness: 905.0000
    standardized fitness: 905.0000
        adjusted fitness: 0.9989
TREE:
 (/ (log convex)
    (- (+ (exp 0.82991) eccentricity)
       (exp (/ perimeter
               (exp (- (/ major
                          (* minor convex))
                       (/ major perimeter)))))))
TREE-equ:
y = (log(convex) / ((exp(0.82991) + eccentricity) - exp((perimeter / exp(((major / (minor * convex)) - (major / perimeter)))))))
