=== BEST-OF-RUN ===
              generation: 0
                   nodes: 40
                   depth: 5
                    hits: 741
TOP INDIVIDUAL:



-- #1 --
                    hits: 741
             raw fitness: 741.0000
    standardized fitness: 741.0000
        adjusted fitness: 0.9987
TREE:
 (- (log (* (+ (- minor area)
               (* area minor))
            (+ (- area eccentricity)
               (- extent perimeter))))
    (* (* (+ (exp -0.58732)
             (/ -0.84888 eccentricity))
          (* (log perimeter)
             (/ major major)))
       (- (log (+ -0.77526 area))
          (log (* area area)))))
TREE-equ:
y = (log((((minor - area) + (area * minor)) * ((area - eccentricity) + (extent - perimeter)))) - (((exp(-0.58732) + (-0.84888 / eccentricity)) * (log(perimeter) * (major / major))) * (log((-0.77526 + area)) - log((area * area)))))
