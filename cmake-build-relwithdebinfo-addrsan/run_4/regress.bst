=== BEST-OF-RUN ===
              generation: 26
                   nodes: 26
                   depth: 5
                    hits: 814
TOP INDIVIDUAL:



-- #1 --
                    hits: 814
             raw fitness: 814.0000
    standardized fitness: 814.0000
        adjusted fitness: 0.9988
TREE:
 (+ (- (- (log minor) minor)
       (- area
          (+ (/ convex convex) eccentricity)))
    (* (* (+ -0.87797 eccentricity)
          (- area extent))
       (* minor
          (exp (log eccentricity)))))
TREE-equ:
y = (((log(minor) - minor) - (area - ((convex / convex) + eccentricity))) + (((-0.87797 + eccentricity) * (area - extent)) * (minor * exp(log(eccentricity)))))
