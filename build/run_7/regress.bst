=== BEST-OF-RUN ===
              generation: 3
                   nodes: 41
                   depth: 8
                    hits: 707
TOP INDIVIDUAL:



-- #1 --
                    hits: 707
             raw fitness: 707.0000
    standardized fitness: 707.0000
        adjusted fitness: 0.9986
TREE:
 (/ (* (/ (* (+ convex major)
             (log perimeter))
          (* (exp extent)
             (exp (- (log (- convex area))
                     (exp (- eccentricity -0.55846))))))
       (log (/ (/ minor convex)
               (exp perimeter))))
    (exp (+ (- major
               (+ (log extent) eccentricity))
            (+ perimeter
               (log area)))))
TREE-equ:
y = (((((convex + major) * log(perimeter)) / (exp(extent) * exp((log((convex - area)) - exp((eccentricity - -0.55846)))))) * log(((minor / convex) / exp(perimeter)))) / exp(((major - (log(extent) + eccentricity)) + (perimeter + log(area)))))
