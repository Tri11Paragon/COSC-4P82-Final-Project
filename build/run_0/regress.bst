=== BEST-OF-RUN ===
              generation: 7
                   nodes: 44
                   depth: 10
                    hits: 878
TOP INDIVIDUAL:



-- #1 --
                    hits: 878
             raw fitness: 878.0000
    standardized fitness: 878.0000
        adjusted fitness: 0.9989
TREE:
 (log (/ (- (log (/ (/ extent -0.03421)
                    (- major minor)))
            (+ (* perimeter -0.22608)
               (exp (log minor))))
         (log (* (* (- convex area)
                    (* (* (/ -0.81500 minor)
                          (/ extent
                             (+ (exp major)
                                (+ major eccentricity))))
                       (exp perimeter)))
                 (+ (exp perimeter)
                    (log area))))))
TREE-equ:
y = log(((log(((extent / -0.03421) / (major - minor))) - ((perimeter * -0.22608) + exp(log(minor)))) / log((((convex - area) * (((-0.81500 / minor) * (extent / (exp(major) + (major + eccentricity)))) * exp(perimeter))) * (exp(perimeter) + log(area))))))
