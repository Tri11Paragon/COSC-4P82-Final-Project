=== BEST-OF-RUN ===
              generation: 11
                   nodes: 43
                   depth: 6
                    hits: 778
TOP INDIVIDUAL:



-- #1 --
                    hits: 778
             raw fitness: 778.0000
    standardized fitness: 778.0000
        adjusted fitness: 0.9987
TREE:
 (- (/ (+ (* (/ (+ extent major)
                (/ extent extent))
             (log (log major)))
          (log (/ -0.94285
                  (+ extent convex))))
       (/ (+ (* (+ minor extent)
                (log minor))
             (log (/ area minor)))
          (+ (* eccentricity
                (log minor))
             (exp (+ extent -0.03985)))))
    (log minor))
TREE-equ:
y = ((((((extent + major) / (extent / extent)) * log(log(major))) + log((-0.94285 / (extent + convex)))) / ((((minor + extent) * log(minor)) + log((area / minor))) / ((eccentricity * log(minor)) + exp((extent + -0.03985))))) - log(minor))
