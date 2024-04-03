=== BEST-OF-RUN ===
              generation: 0
                   nodes: 55
                   depth: 6
                    hits: 584
TOP INDIVIDUAL:



-- #1 --
                    hits: 584
             raw fitness: 584.0000
    standardized fitness: 584.0000
        adjusted fitness: 0.9983
TREE:
 (+ (+ (exp (- (* (+ extent -0.43461)
                  (exp 0.33948))
               (exp (log eccentricity))))
       (* (log (log (+ eccentricity area)))
          (/ (/ (log convex)
                (/ extent major))
             (* (log perimeter)
                (- 0.61644 extent)))))
    (* (log (log (+ (- eccentricity minor)
                    (/ major 0.24426))))
       (exp (* (/ (exp extent)
                  (log eccentricity))
               (* (* extent eccentricity)
                  (log convex))))))
TREE-equ:
y = ((exp((((extent + -0.43461) * exp(0.33948)) - exp(log(eccentricity)))) + (log(log((eccentricity + area))) * ((log(convex) / (extent / major)) / (log(perimeter) * (0.61644 - extent))))) + (log(log(((eccentricity - minor) + (major / 0.24426)))) * exp(((exp(extent) / log(eccentricity)) * ((extent * eccentricity) * log(convex))))))
