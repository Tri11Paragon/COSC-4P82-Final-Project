=== BEST-OF-RUN ===
              generation: 39
                   nodes: 44
                   depth: 12
                    hits: 933
TOP INDIVIDUAL:



-- #1 --
                    hits: 933
             raw fitness: 933.0000
    standardized fitness: 933.0000
        adjusted fitness: 0.9989
TREE:
 (* (* (log (/ (exp (log (exp perimeter))) major))
       (exp (* (log (/ perimeter minor))
               (- (log (+ 0.40769
                          (* (* convex
                                (- (/ (- major minor) major) eccentricity))
                             (+ (- perimeter minor)
                                (exp (log (* minor eccentricity)))))))
                  (+ extent perimeter)))))
    (- -0.33638 area))
TREE-equ:
y = ((log((exp(log(exp(perimeter))) / major)) * exp((log((perimeter / minor)) * (log((0.40769 + ((convex * (((major - minor) / major) - eccentricity)) * ((perimeter - minor) + exp(log((minor * eccentricity))))))) - (extent + perimeter))))) * (-0.33638 - area))
