=== BEST-OF-RUN ===
              generation: 48
                   nodes: 44
                   depth: 13
                    hits: 900
TOP INDIVIDUAL:



-- #1 --
                    hits: 900
             raw fitness: 900.0000
    standardized fitness: 900.0000
        adjusted fitness: 0.9989
TREE:
 (/ perimeter
    (+ (* (/ (* (exp minor) convex)
             (log (* (log (- (- (/ convex
                                   (+ eccentricity
                                      (* area area)))
                                (+ (/ minor eccentricity) major))
                             (* (exp (/ convex
                                        (* (- major 0.68855) convex))) major))) -0.16223))) area)
       (log (+ (- perimeter minor) -0.19669))))
TREE-equ:
y = (perimeter / ((((exp(minor) * convex) / log((log((((convex / (eccentricity + (area * area))) - ((minor / eccentricity) + major)) - (exp((convex / ((major - 0.68855) * convex))) * major))) * -0.16223))) * area) + log(((perimeter - minor) + -0.19669))))
