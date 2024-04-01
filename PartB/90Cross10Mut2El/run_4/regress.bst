=== BEST-OF-RUN ===
              generation: 30
                   nodes: 36
                   depth: 9
                    hits: 908
TOP INDIVIDUAL:



-- #1 --
                    hits: 908
             raw fitness: 908.0000
    standardized fitness: 908.0000
        adjusted fitness: 0.9989
TREE:
 (log (exp (+ extent
              (- (/ (/ (* (log (log eccentricity))
                          (* major minor))
                       (log (- (- area major)
                               (* major eccentricity))))
                    (+ (log -0.21330)
                       (log (/ (* convex convex)
                               (+ minor major)))))
                 (* eccentricity perimeter)))))
TREE-equ:
y = log(exp((extent + ((((log(log(eccentricity)) * (major * minor)) / log(((area - major) - (major * eccentricity)))) / (log(-0.21330) + log(((convex * convex) / (minor + major))))) - (eccentricity * perimeter)))))
