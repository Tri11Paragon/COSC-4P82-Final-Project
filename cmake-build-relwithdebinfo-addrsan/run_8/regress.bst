=== BEST-OF-RUN ===
              generation: 0
                   nodes: 23
                   depth: 4
                    hits: 711
TOP INDIVIDUAL:



-- #1 --
                    hits: 711
             raw fitness: 711.0000
    standardized fitness: 711.0000
        adjusted fitness: 0.9986
TREE:
 (/ (/ (* (+ convex perimeter)
          (log eccentricity))
       (exp (/ 0.37166 extent)))
    (+ (/ (- perimeter perimeter)
          (exp 0.85091))
       (exp (/ perimeter extent))))
TREE-equ:
y = ((((convex + perimeter) * log(eccentricity)) / exp((0.37166 / extent))) / (((perimeter - perimeter) / exp(0.85091)) + exp((perimeter / extent))))
