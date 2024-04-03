=== BEST-OF-RUN ===
              generation: 6
                   nodes: 71
                   depth: 10
                    hits: 756
TOP INDIVIDUAL:



-- #1 --
                    hits: 756
             raw fitness: 756.0000
    standardized fitness: 756.0000
        adjusted fitness: 0.9987
TREE:
 (- (/ (exp (exp (log (* -0.43888 perimeter))))
       (* (exp (* (/ minor convex)
                  (+ extent extent)))
          (- (/ (log -0.16812)
                (exp perimeter))
             (/ (- 0.62636 -0.65079)
                (log convex)))))
    (* (- (+ (log eccentricity)
             (* (+ major convex)
                (log major)))
          (exp (* (exp (log (/ (- (* major major)
                                  (+ area extent))
                               (* (log perimeter)
                                  (exp eccentricity)))))
                  (/ extent minor))))
       (log (* (exp convex)
               (* (* convex minor)
                  (/ major area))))))
TREE-equ:
y = ((exp(exp(log((-0.43888 * perimeter)))) / (exp(((minor / convex) * (extent + extent))) * ((log(-0.16812) / exp(perimeter)) - ((0.62636 - -0.65079) / log(convex))))) - (((log(eccentricity) + ((major + convex) * log(major))) - exp((exp(log((((major * major) - (area + extent)) / (log(perimeter) * exp(eccentricity))))) * (extent / minor)))) * log((exp(convex) * ((convex * minor) * (major / area))))))
