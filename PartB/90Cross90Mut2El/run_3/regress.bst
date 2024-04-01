=== BEST-OF-RUN ===
              generation: 50
                   nodes: 18
                   depth: 6
                    hits: 878
TOP INDIVIDUAL:



-- #1 --
                    hits: 878
             raw fitness: 878.0000
    standardized fitness: 878.0000
        adjusted fitness: 0.9989
TREE:
 (/ (exp major)
    (/ (exp (+ (- (- perimeter -0.39302) major) perimeter))
       (- (exp extent)
          (* eccentricity area))))
TREE-equ:
y = (exp(major) / (exp((((perimeter - -0.39302) - major) + perimeter)) / (exp(extent) - (eccentricity * area))))
