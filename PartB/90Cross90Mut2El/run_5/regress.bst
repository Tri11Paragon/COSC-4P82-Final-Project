=== BEST-OF-RUN ===
              generation: 25
                   nodes: 37
                   depth: 5
                    hits: 840
TOP INDIVIDUAL:



-- #1 --
                    hits: 840
             raw fitness: 840.0000
    standardized fitness: 840.0000
        adjusted fitness: 0.9988
TREE:
 (+ (/ (+ (/ (+ major perimeter)
             (- minor major))
          (- (/ eccentricity area)
             (exp eccentricity)))
       (exp (/ (/ extent eccentricity)
               (* convex perimeter))))
    (log (- (/ area
               (+ minor minor))
            (/ (+ area area)
               (log minor)))))
TREE-equ:
y = (((((major + perimeter) / (minor - major)) + ((eccentricity / area) - exp(eccentricity))) / exp(((extent / eccentricity) / (convex * perimeter)))) + log(((area / (minor + minor)) - ((area + area) / log(minor)))))
