=== BEST-OF-RUN ===
              generation: 19
                   nodes: 40
                   depth: 7
                    hits: 897
TOP INDIVIDUAL:



-- #1 --
                    hits: 897
             raw fitness: 897.0000
    standardized fitness: 897.0000
        adjusted fitness: 0.9989
TREE:
 (+ (exp (log (- (log (* major
                         (exp -0.00203)))
                 (+ (log extent)
                    (- area perimeter)))))
    (+ (+ (* (log (/ extent convex)) perimeter)
          (- (- (/ perimeter perimeter)
                (/ minor 0.01145)) minor))
       (* (log (log (/ convex area)))
          (log area))))
TREE-equ:
y = (exp(log((log((major * exp(-0.00203))) - (log(extent) + (area - perimeter))))) + (((log((extent / convex)) * perimeter) + (((perimeter / perimeter) - (minor / 0.01145)) - minor)) + (log(log((convex / area))) * log(area))))
