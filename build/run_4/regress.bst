=== BEST-OF-RUN ===
              generation: 6
                   nodes: 13
                   depth: 7
                    hits: 713
TOP INDIVIDUAL:



-- #1 --
                    hits: 713
             raw fitness: 713.0000
    standardized fitness: 713.0000
        adjusted fitness: 0.9986
TREE:
 (+ perimeter
    (log (/ perimeter
            (exp (/ major
                    (/ minor
                       (* perimeter extent)))))))
TREE-equ:
y = (perimeter + log((perimeter / exp((major / (minor / (perimeter * extent)))))))
