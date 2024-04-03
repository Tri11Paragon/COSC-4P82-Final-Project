=== BEST-OF-RUN ===
              generation: 5
                   nodes: 19
                   depth: 5
                    hits: 801
TOP INDIVIDUAL:



-- #1 --
                    hits: 801
             raw fitness: 801.0000
    standardized fitness: 801.0000
        adjusted fitness: 0.9988
TREE:
 (/ (log extent)
    (* (exp major)
       (* (/ (exp perimeter)
             (exp eccentricity))
          (* (+ convex extent)
             (* convex convex)))))
TREE-equ:
y = (log(extent) / (exp(major) * ((exp(perimeter) / exp(eccentricity)) * ((convex + extent) * (convex * convex)))))
