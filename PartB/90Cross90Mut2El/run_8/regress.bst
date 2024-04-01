=== BEST-OF-RUN ===
              generation: 34
                   nodes: 33
                   depth: 6
                    hits: 921
TOP INDIVIDUAL:



-- #1 --
                    hits: 921
             raw fitness: 921.0000
    standardized fitness: 921.0000
        adjusted fitness: 0.9989
TREE:
 (* (/ (* (* -0.00391 convex) eccentricity)
       (* (exp (* (+ -0.39389 area)
                  (/ perimeter area))) convex))
    (/ (exp (log (- eccentricity extent)))
       (+ (exp (+ major minor))
          (log (- convex
                  (exp major))))))
TREE-equ:
y = ((((-0.00391 * convex) * eccentricity) / (exp(((-0.39389 + area) * (perimeter / area))) * convex)) * (exp(log((eccentricity - extent))) / (exp((major + minor)) + log((convex - exp(major))))))
