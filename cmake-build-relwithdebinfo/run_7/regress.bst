=== BEST-OF-RUN ===
              generation: 2
                   nodes: 56
                   depth: 10
                    hits: 690
TOP INDIVIDUAL:



-- #1 --
                    hits: 690
             raw fitness: 690.0000
    standardized fitness: 690.0000
        adjusted fitness: 0.9986
TREE:
 (* (/ (exp (exp (exp extent)))
       (+ (- (exp (log (/ extent
                          (/ (+ (- extent major)
                                (log area))
                             (* (- convex convex)
                                (+ area -0.68090)))))) -0.81604)
          (/ (* -0.08088 major)
             (log convex))))
    (/ (/ (/ (exp perimeter)
             (/ major eccentricity))
          (/ (/ eccentricity eccentricity)
             (* area -0.99052)))
       (exp (/ (- minor convex)
               (* eccentricity perimeter)))))
TREE-equ:
y = ((exp(exp(exp(extent))) / ((exp(log((extent / (((extent - major) + log(area)) / ((convex - convex) * (area + -0.68090)))))) - -0.81604) + ((-0.08088 * major) / log(convex)))) * (((exp(perimeter) / (major / eccentricity)) / ((eccentricity / eccentricity) / (area * -0.99052))) / exp(((minor - convex) / (eccentricity * perimeter)))))
