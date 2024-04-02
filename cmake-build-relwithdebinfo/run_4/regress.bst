=== BEST-OF-RUN ===
              generation: 6
                   nodes: 22
                   depth: 5
                    hits: 638
TOP INDIVIDUAL:



-- #1 --
                    hits: 638
             raw fitness: 638.0000
    standardized fitness: 638.0000
        adjusted fitness: 0.9984
TREE:
 (* (exp (/ (/ convex minor)
            (log extent)))
    (- (exp (- (/ extent major)
               (* -0.10985 minor)))
       (exp (log (- extent area)))))
TREE-equ:
y = (exp(((convex / minor) / log(extent))) * (exp(((extent / major) - (-0.10985 * minor))) - exp(log((extent - area)))))
