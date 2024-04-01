=== BEST-OF-RUN ===
              generation: 11
                   nodes: 44
                   depth: 6
                    hits: 908
TOP INDIVIDUAL:



-- #1 --
                    hits: 908
             raw fitness: 908.0000
    standardized fitness: 908.0000
        adjusted fitness: 0.9989
TREE:
 (log (/ (/ (/ (/ (/ area area)
                  (- 0.74146 minor))
               (* (exp minor)
                  (/ convex area)))
            (+ (exp (+ major perimeter)) eccentricity))
         (/ (+ (- (/ minor perimeter) 0.73904)
               (- (- extent eccentricity)
                  (log extent)))
            (exp (+ (- minor eccentricity)
                    (+ -0.77127 eccentricity))))))
TREE-equ:
y = log((((((area / area) / (0.74146 - minor)) / (exp(minor) * (convex / area))) / (exp((major + perimeter)) + eccentricity)) / ((((minor / perimeter) - 0.73904) + ((extent - eccentricity) - log(extent))) / exp(((minor - eccentricity) + (-0.77127 + eccentricity))))))
