=== BEST-OF-RUN ===
              generation: 2
                   nodes: 18
                   depth: 5
                    hits: 902
TOP INDIVIDUAL:



-- #1 --
                    hits: 902
             raw fitness: 902.0000
    standardized fitness: 902.0000
        adjusted fitness: 0.9989
TREE:
 (/ (- (log extent)
       (* convex eccentricity))
    (exp (- (+ (+ extent perimeter)
               (* 0.56548 perimeter))
            (log convex))))
TREE-equ:
y = ((log(extent) - (convex * eccentricity)) / exp((((extent + perimeter) + (0.56548 * perimeter)) - log(convex))))
