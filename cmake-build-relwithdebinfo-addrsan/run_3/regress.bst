=== BEST-OF-RUN ===
              generation: 2
                   nodes: 25
                   depth: 5
                    hits: 757
TOP INDIVIDUAL:



-- #1 --
                    hits: 757
             raw fitness: 757.0000
    standardized fitness: 757.0000
        adjusted fitness: 0.9987
TREE:
 (+ (+ -0.18535
       (log (exp (* eccentricity major))))
    (- (* (+ (- eccentricity minor)
             (/ eccentricity eccentricity))
          (log (/ extent 0.11096)))
       (log (+ area convex))))
TREE-equ:
y = ((-0.18535 + log(exp((eccentricity * major)))) + ((((eccentricity - minor) + (eccentricity / eccentricity)) * log((extent / 0.11096))) - log((area + convex))))
