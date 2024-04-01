=== BEST-OF-RUN ===
              generation: 42
                   nodes: 16
                   depth: 7
                    hits: 905
TOP INDIVIDUAL:



-- #1 --
                    hits: 905
             raw fitness: 905.0000
    standardized fitness: 905.0000
        adjusted fitness: 0.9989
TREE:
 (/ (exp minor)
    (/ -0.32253
       (/ eccentricity
          (exp (- perimeter
                  (* (log -0.29408)
                     (+ extent major)))))))
TREE-equ:
y = (exp(minor) / (-0.32253 / (eccentricity / exp((perimeter - (log(-0.29408) * (extent + major)))))))
