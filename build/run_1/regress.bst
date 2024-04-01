=== BEST-OF-RUN ===
              generation: 4
                   nodes: 93
                   depth: 11
                    hits: 876
TOP INDIVIDUAL:



-- #1 --
                    hits: 876
             raw fitness: 876.0000
    standardized fitness: 876.0000
        adjusted fitness: 0.9989
TREE:
 (- (/ (log (/ (/ (exp perimeter)
                  (* major extent))
               (exp (- major perimeter))))
       (* (- (exp (exp eccentricity))
             (+ (- -0.01190 eccentricity)
                (log major)))
          (exp (exp (- eccentricity eccentricity)))))
    (- (log (+ (+ (exp perimeter)
                  (+ extent minor))
               (* (/ eccentricity major)
                  (log (* (log (- (exp (exp extent))
                                  (- (- extent eccentricity)
                                     (* minor major))))
                          (exp (* perimeter extent)))))))
       (/ (exp (+ (/ extent area)
                  (- 0.47920 extent)))
          (+ (- (* perimeter extent)
                (exp eccentricity))
             (log (/ (/ major perimeter)
                     (/ (+ (log convex)
                           (log convex))
                        (/ (log convex)
                           (log 0.63283)))))))))
TREE-equ:
y = ((log(((exp(perimeter) / (major * extent)) / exp((major - perimeter)))) / ((exp(exp(eccentricity)) - ((-0.01190 - eccentricity) + log(major))) * exp(exp((eccentricity - eccentricity))))) - (log(((exp(perimeter) + (extent + minor)) + ((eccentricity / major) * log((log((exp(exp(extent)) - ((extent - eccentricity) - (minor * major)))) * exp((perimeter * extent))))))) - (exp(((extent / area) + (0.47920 - extent))) / (((perimeter * extent) - exp(eccentricity)) + log(((major / perimeter) / ((log(convex) + log(convex)) / (log(convex) / log(0.63283)))))))))
