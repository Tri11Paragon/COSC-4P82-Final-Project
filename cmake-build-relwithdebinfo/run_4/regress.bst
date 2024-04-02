=== BEST-OF-RUN ===
              generation: 4
                   nodes: 38
                   depth: 8
                    hits: 532
TOP INDIVIDUAL:



-- #1 --
                    hits: 532
             raw fitness: 532.0000
    standardized fitness: 532.0000
        adjusted fitness: 0.9981
TREE:
 (* (- (exp (/ (/ extent eccentricity)
               (exp convex)))
       (+ (- (/ 0.10974 extent)
             (/ area 0.97881))
          (log (/ perimeter minor))))
    (log (- (/ (* (- extent
                     (log (exp extent))) convex)
               (* eccentricity perimeter))
            (log (/ minor major)))))
TREE-equ:
y = ((exp(((extent / eccentricity) / exp(convex))) - (((0.10974 / extent) - (area / 0.97881)) + log((perimeter / minor)))) * log(((((extent - log(exp(extent))) * convex) / (eccentricity * perimeter)) - log((minor / major)))))
