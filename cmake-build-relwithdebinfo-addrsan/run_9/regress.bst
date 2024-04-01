=== BEST-OF-RUN ===
              generation: 4
                   nodes: 33
                   depth: 7
                    hits: 789
TOP INDIVIDUAL:



-- #1 --
                    hits: 789
             raw fitness: 789.0000
    standardized fitness: 789.0000
        adjusted fitness: 0.9987
TREE:
 (/ (log (/ (log (exp major)) area))
    (* (/ (log (/ eccentricity 0.88294))
          (- (+ minor area)
             (- (* (+ area perimeter) minor) major)))
       (* (exp (/ major extent))
          (exp (+ extent 0.07221)))))
TREE-equ:
y = (log((log(exp(major)) / area)) / ((log((eccentricity / 0.88294)) / ((minor + area) - (((area + perimeter) * minor) - major))) * (exp((major / extent)) * exp((extent + 0.07221)))))
