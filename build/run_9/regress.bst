=== BEST-OF-RUN ===
              generation: 23
                   nodes: 23
                   depth: 6
                    hits: 945
TOP INDIVIDUAL:



-- #1 --
                    hits: 945
             raw fitness: 945.0000
    standardized fitness: 945.0000
        adjusted fitness: 0.9989
TREE:
 (/ (+ (/ (* perimeter -0.79452)
          (log major)) extent)
    (* (exp (- (log (* area convex))
               (* (exp 0.63537)
                  (* eccentricity perimeter)))) 0.37993))
TREE-equ:
y = ((((perimeter * -0.79452) / log(major)) + extent) / (exp((log((area * convex)) - (exp(0.63537) * (eccentricity * perimeter)))) * 0.37993))
