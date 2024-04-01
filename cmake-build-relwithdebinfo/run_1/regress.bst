=== BEST-OF-RUN ===
              generation: 2
                   nodes: 26
                   depth: 6
                    hits: 835
TOP INDIVIDUAL:



-- #1 --
                    hits: 835
             raw fitness: 835.0000
    standardized fitness: 835.0000
        adjusted fitness: 0.9988
TREE:
 (/ major
    (* (- (exp (+ perimeter
                  (/ major eccentricity)))
          (/ (- (* minor convex)
                (+ extent convex))
             (/ (exp minor)
                (log extent))))
       (- extent area)))
TREE-equ:
y = (major / ((exp((perimeter + (major / eccentricity))) - (((minor * convex) - (extent + convex)) / (exp(minor) / log(extent)))) * (extent - area)))
