=== BEST-OF-RUN ===
              generation: 0
                   nodes: 55
                   depth: 5
                    hits: 658
TOP INDIVIDUAL:



-- #1 --
                    hits: 658
             raw fitness: 658.0000
    standardized fitness: 658.0000
        adjusted fitness: 0.9985
TREE:
 (/ (+ (/ (/ (+ eccentricity convex)
             (+ perimeter extent))
          (- (/ perimeter convex)
             (log 0.69297)))
       (- (log (exp perimeter))
          (/ (- 0.21163 0.50212)
             (/ extent minor))))
    (+ (+ (* (- area extent)
             (+ 0.07852 extent))
          (/ (/ area extent)
             (+ extent convex)))
       (- (/ (* area major)
             (log major))
          (* (exp major)
             (exp perimeter)))))
TREE-equ:
y = (((((eccentricity + convex) / (perimeter + extent)) / ((perimeter / convex) - log(0.69297))) + (log(exp(perimeter)) - ((0.21163 - 0.50212) / (extent / minor)))) / ((((area - extent) * (0.07852 + extent)) + ((area / extent) / (extent + convex))) + (((area * major) / log(major)) - (exp(major) * exp(perimeter)))))
