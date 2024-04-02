=== BEST-OF-RUN ===
              generation: 10
                   nodes: 28
                   depth: 8
                    hits: 881
TOP INDIVIDUAL:



-- #1 --
                    hits: 881
             raw fitness: 881.0000
    standardized fitness: 881.0000
        adjusted fitness: 0.9989
TREE:
 (+ (- major
       (log (/ extent
               (exp (* -0.05728
                       (+ area major))))))
    (exp (exp (* (+ area perimeter)
                 (* minor
                    (- (+ major extent)
                       (exp (* eccentricity area))))))))
TREE-equ:
y = ((major - log((extent / exp((-0.05728 * (area + major)))))) + exp(exp(((area + perimeter) * (minor * ((major + extent) - exp((eccentricity * area))))))))
