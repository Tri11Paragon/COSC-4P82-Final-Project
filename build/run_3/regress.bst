=== BEST-OF-RUN ===
              generation: 16
                   nodes: 30
                   depth: 7
                    hits: 914
TOP INDIVIDUAL:



-- #1 --
                    hits: 914
             raw fitness: 914.0000
    standardized fitness: 914.0000
        adjusted fitness: 0.9989
TREE:
 (log (/ (/ (/ (/ major convex) extent)
            (+ (exp major)
               (+ extent extent)))
         (- (exp (+ minor perimeter))
            (/ (+ area
                  (+ (+ eccentricity extent) eccentricity))
               (+ extent extent)))))
TREE-equ:
y = log(((((major / convex) / extent) / (exp(major) + (extent + extent))) / (exp((minor + perimeter)) - ((area + ((eccentricity + extent) + eccentricity)) / (extent + extent)))))
