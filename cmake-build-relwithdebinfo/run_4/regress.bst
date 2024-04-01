=== BEST-OF-RUN ===
              generation: 2
                   nodes: 23
                   depth: 5
                    hits: 771
TOP INDIVIDUAL:



-- #1 --
                    hits: 771
             raw fitness: 771.0000
    standardized fitness: 771.0000
        adjusted fitness: 0.9987
TREE:
 (log (/ (exp (- (- eccentricity perimeter)
                 (* major eccentricity)))
         (/ (* (exp extent)
               (exp minor))
            (+ (- perimeter area)
               (- minor convex)))))
TREE-equ:
y = log((exp(((eccentricity - perimeter) - (major * eccentricity))) / ((exp(extent) * exp(minor)) / ((perimeter - area) + (minor - convex)))))
