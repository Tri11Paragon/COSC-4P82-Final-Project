=== BEST-OF-RUN ===
              generation: 24
                   nodes: 25
                   depth: 6
                    hits: 810
TOP INDIVIDUAL:



-- #1 --
                    hits: 810
             raw fitness: 810.0000
    standardized fitness: 810.0000
        adjusted fitness: 0.9988
TREE:
 (+ (+ (- (* -0.49488
             (- perimeter minor))
          (exp (log area)))
       (/ (log perimeter) extent))
    (* (* major
          (* (- (log major) extent) major)) 0.08088))
TREE-equ:
y = ((((-0.49488 * (perimeter - minor)) - exp(log(area))) + (log(perimeter) / extent)) + ((major * ((log(major) - extent) * major)) * 0.08088))
