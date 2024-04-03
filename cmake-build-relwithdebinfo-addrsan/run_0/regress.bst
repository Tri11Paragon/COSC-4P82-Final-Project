=== BEST-OF-RUN ===
              generation: 6
                   nodes: 28
                   depth: 5
                    hits: 706
TOP INDIVIDUAL:



-- #1 --
                    hits: 706
             raw fitness: 706.0000
    standardized fitness: 706.0000
        adjusted fitness: 0.9986
TREE:
 (+ (* (/ (- (* eccentricity extent) convex) area)
       (log (exp (/ area convex))))
    (- (+ (+ (/ major minor)
             (log extent)) extent)
       (exp (log (+ extent extent)))))
TREE-equ:
y = (((((eccentricity * extent) - convex) / area) * log(exp((area / convex)))) + ((((major / minor) + log(extent)) + extent) - exp(log((extent + extent)))))
