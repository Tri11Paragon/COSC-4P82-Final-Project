=== BEST-OF-RUN ===
              generation: 1
                   nodes: 68
                   depth: 6
                    hits: 648
TOP INDIVIDUAL:



-- #1 --
                    hits: 648
             raw fitness: 648.0000
    standardized fitness: 648.0000
        adjusted fitness: 0.9985
TREE:
 (- (* (* (/ (exp (- 0.16610 area))
             (log (* area minor)))
          (* (/ (/ extent extent)
                (- extent area))
             (+ (exp 0.33696)
                (+ convex major))))
       (exp (* (+ (- extent area)
                  (* -0.07066 minor))
               (exp (exp area)))))
    (/ (log (log (log (exp extent))))
       (- (- (+ (exp eccentricity) 0.62824)
             (* (/ -0.57147 convex)
                (* minor extent)))
          (+ (log (log area))
             (+ (/ minor major)
                (/ major perimeter))))))
TREE-equ:
y = ((((exp((0.16610 - area)) / log((area * minor))) * (((extent / extent) / (extent - area)) * (exp(0.33696) + (convex + major)))) * exp((((extent - area) + (-0.07066 * minor)) * exp(exp(area))))) - (log(log(log(exp(extent)))) / (((exp(eccentricity) + 0.62824) - ((-0.57147 / convex) * (minor * extent))) - (log(log(area)) + ((minor / major) + (major / perimeter))))))
