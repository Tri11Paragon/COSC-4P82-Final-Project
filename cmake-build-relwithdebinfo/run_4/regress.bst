=== BEST-OF-RUN ===
              generation: 4
                   nodes: 52
                   depth: 8
                    hits: 720
TOP INDIVIDUAL:



-- #1 --
                    hits: 720
             raw fitness: 720.0000
    standardized fitness: 720.0000
        adjusted fitness: 0.9986
TREE:
 (* (* (exp (* (log (+ convex -0.25821))
               (exp (- extent convex))))
       (+ eccentricity extent))
    (- (/ (log (log (/ minor convex)))
          (log (/ (log extent)
                  (exp extent))))
       (- (* (/ (+ convex extent)
                (+ 0.46105 extent))
             (+ (+ perimeter extent)
                (exp major)))
          (log (+ (* area
                     (exp (+ major perimeter))) area)))))
TREE-equ:
y = ((exp((log((convex + -0.25821)) * exp((extent - convex)))) * (eccentricity + extent)) * ((log(log((minor / convex))) / log((log(extent) / exp(extent)))) - ((((convex + extent) / (0.46105 + extent)) * ((perimeter + extent) + exp(major))) - log(((area * exp((major + perimeter))) + area)))))
