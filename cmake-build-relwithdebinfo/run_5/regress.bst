=== BEST-OF-RUN ===
              generation: 15
                   nodes: 23
                   depth: 5
                    hits: 889
TOP INDIVIDUAL:



-- #1 --
                    hits: 889
             raw fitness: 889.0000
    standardized fitness: 889.0000
        adjusted fitness: 0.9989
TREE:
 (/ (exp (- (/ extent extent) perimeter))
    (- (exp (+ (log minor)
               (/ area major)))
       (* (* area eccentricity)
          (exp (- perimeter major)))))
TREE-equ:
y = (exp(((extent / extent) - perimeter)) / (exp((log(minor) + (area / major))) - ((area * eccentricity) * exp((perimeter - major)))))
