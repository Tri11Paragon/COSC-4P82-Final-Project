=== BEST-OF-RUN ===
              generation: 20
                   nodes: 21
                   depth: 6
                    hits: 728
TOP INDIVIDUAL:



-- #1 --
                    hits: 728
             raw fitness: 728.0000
    standardized fitness: 728.0000
        adjusted fitness: 0.9986
TREE:
 (* area
    (/ area
       (+ (* (exp (/ perimeter perimeter))
             (/ (* area eccentricity) area))
          (- (exp (/ perimeter minor)) major))))
TREE-equ:
y = (area * (area / ((exp((perimeter / perimeter)) * ((area * eccentricity) / area)) + (exp((perimeter / minor)) - major))))
