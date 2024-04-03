=== BEST-OF-RUN ===
              generation: 9
                   nodes: 36
                   depth: 10
                    hits: 681
TOP INDIVIDUAL:



-- #1 --
                    hits: 681
             raw fitness: 681.0000
    standardized fitness: 681.0000
        adjusted fitness: 0.9985
TREE:
 (- area
    (- major
       (- (* minor eccentricity)
          (* (/ (* (+ (log (- (/ major perimeter)
                              (log extent)))
                      (log perimeter))
                   (* (log (exp perimeter))
                      (/ (+ perimeter 0.97953)
                         (+ major
                            (* minor extent))))) major) perimeter))))
TREE-equ:
y = (area - (major - ((minor * eccentricity) - ((((log(((major / perimeter) - log(extent))) + log(perimeter)) * (log(exp(perimeter)) * ((perimeter + 0.97953) / (major + (minor * extent))))) / major) * perimeter))))
