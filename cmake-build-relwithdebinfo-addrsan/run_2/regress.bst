=== BEST-OF-RUN ===
              generation: 4
                   nodes: 65
                   depth: 7
                    hits: 636
TOP INDIVIDUAL:



-- #1 --
                    hits: 636
             raw fitness: 636.0000
    standardized fitness: 636.0000
        adjusted fitness: 0.9984
TREE:
 (+ (/ (- (exp (- (+ eccentricity perimeter)
                  (exp perimeter)))
          (log (+ (- minor major)
                  (- area area))))
       (- (log (- (/ extent major)
                  (+ eccentricity major)))
          (exp convex)))
    (- (log (/ (+ (- area
                     (/ extent major))
                  (- minor convex))
               (exp (log minor))))
       (- (exp (/ (- convex area)
                  (* area extent)))
          (* (/ (* convex perimeter)
                (/ (exp area) -0.16825))
             (+ extent 0.93211)))))
TREE-equ:
y = (((exp(((eccentricity + perimeter) - exp(perimeter))) - log(((minor - major) + (area - area)))) / (log(((extent / major) - (eccentricity + major))) - exp(convex))) + (log((((area - (extent / major)) + (minor - convex)) / exp(log(minor)))) - (exp(((convex - area) / (area * extent))) - (((convex * perimeter) / (exp(area) / -0.16825)) * (extent + 0.93211)))))
