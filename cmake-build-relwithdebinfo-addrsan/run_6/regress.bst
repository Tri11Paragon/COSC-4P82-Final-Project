=== BEST-OF-RUN ===
              generation: 37
                   nodes: 27
                   depth: 9
                    hits: 806
TOP INDIVIDUAL:



-- #1 --
                    hits: 806
             raw fitness: 806.0000
    standardized fitness: 806.0000
        adjusted fitness: 0.9988
TREE:
 (* (log (/ (- minor
               (+ (* major eccentricity)
                  (log (* (+ (/ eccentricity convex) perimeter) eccentricity))))
            (+ (log extent)
               (+ minor extent))))
    (exp (log (log eccentricity))))
TREE-equ:
y = (log(((minor - ((major * eccentricity) + log((((eccentricity / convex) + perimeter) * eccentricity)))) / (log(extent) + (minor + extent)))) * exp(log(log(eccentricity))))
