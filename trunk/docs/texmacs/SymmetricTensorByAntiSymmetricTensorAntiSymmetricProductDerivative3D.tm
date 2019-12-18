<TeXmacs|1.99.2>

<style|<tuple|old-generic|maxima|french>>

<\body>
  <\session|maxima|default>
    <\output>
      \;

      Maxima 5.27.0 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|input>
      <\unfolded-io>
        <with|color|red|(<with|math-font-family|rm|%i>8) >
      <|unfolded-io>
        F:matrix([F0,F3/sqrt(2),F4/sqrt(2)],[F3/sqrt(2),F1,F5/sqrt(2)],[F4/sqrt(2),F5/sqrt(2),F2])
      <|unfolded-io>
        <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
        >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|F5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
      </unfolded-io>

      <\unfolded-io>
        <with|color|red|(<with|math-font-family|rm|%i>9) >
      <|unfolded-io>
        G:matrix([0,G3,G4],[-G3,0,G5],[-G4,-G5,0])
      <|unfolded-io>
        \;

        \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o9>)
        >><matrix|<tformat|<table|<row|<cell|0>|<cell|<with|math-font-family|rm|G3>>|<cell|<with|math-font-family|rm|G4>>>|<row|<cell|-<with|math-font-family|rm|G3>>|<cell|0>|<cell|<with|math-font-family|rm|G5>>>|<row|<cell|-<with|math-font-family|rm|G4>>|<cell|-<with|math-font-family|rm|G5>>|<cell|0>>>>>>>
      </unfolded-io>

      <\unfolded-io>
        <with|color|red|(<with|math-font-family|rm|%i>10) >
      <|unfolded-io>
        prod:F.G-G.F
      <|unfolded-io>
        <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
        >><matrix|<tformat|<table|<row|<cell|-<sqrt|2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>-<sqrt|2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>>|<cell|-<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G5>|<sqrt|2>>-<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G4>|<sqrt|2>>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>>|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G5>|<sqrt|2>>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G4>-<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G3>|<sqrt|2>>>>|<row|<cell|-<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G5>|<sqrt|2>>-<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G4>|<sqrt|2>>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>>|<cell|<sqrt|2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>-<sqrt|2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>>|<cell|-<with|math-font-family|rm|F2>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G5>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>|<sqrt|2>>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G5>|<sqrt|2>>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G4>-<frac|<with|math-font-family|rm|F5>*<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|-<with|math-font-family|rm|F2>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G5>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>|<sqrt|2>>+<frac|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|<sqrt|2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>+<sqrt|2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>>>>>>>>
      </unfolded-io>

      <\unfolded-io>
        <with|color|red|(<with|math-font-family|rm|%i>11) >
      <|unfolded-io>
        pv:fullratsimp(transpose(matrix([prod[1,1],prod[2,2],prod[3,3],sqrt(2)*prod[1,2],sqrt(2)*prod[1,3],sqrt(2)*prod[2,3]])))
      <|unfolded-io>
        \;

        \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
        >><matrix|<tformat|<table|<row|<cell|-<sqrt|2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>-<sqrt|2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>>>|<row|<cell|<sqrt|2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>-<sqrt|2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>>>|<row|<cell|<sqrt|2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|G5>+<sqrt|2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|G4>>>|<row|<cell|-<with|math-font-family|rm|F4>*<with|math-font-family|rm|G5>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|G4>+<around*|(|<sqrt|2>*<with|math-font-family|rm|F0>-<sqrt|2>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|G3>>>|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G5>+<around*|(|<sqrt|2>*<with|math-font-family|rm|F0>-<sqrt|2>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|G4>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|G3>>>|<row|<cell|<around*|(|<sqrt|2>*<with|math-font-family|rm|F1>-<sqrt|2>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|G5>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>>>>>>>>
      </unfolded-io>

      <\unfolded-io>
        <with|color|red|(<with|math-font-family|rm|%i>12) >
      <|unfolded-io>
        J1:fullratsimp(addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3),diff(pv,F4),diff(pv,F5)))
      <|unfolded-io>
        <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
        >><matrix|<tformat|<table|<row|<cell|0>|<cell|0>|<cell|0>|<cell|-<sqrt|2>*<with|math-font-family|rm|G3>>|<cell|-<sqrt|2>*<with|math-font-family|rm|G4>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|G3>>|<cell|0>|<cell|-<sqrt|2>*<with|math-font-family|rm|G5>>>|<row|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|G4>>|<cell|<sqrt|2>*<with|math-font-family|rm|G5>>>|<row|<cell|<sqrt|2>*<with|math-font-family|rm|G3>>|<cell|-<sqrt|2>*<with|math-font-family|rm|G3>>|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|G5>>|<cell|-<with|math-font-family|rm|G4>>>|<row|<cell|<sqrt|2>*<with|math-font-family|rm|G4>>|<cell|0>|<cell|-<sqrt|2>*<with|math-font-family|rm|G4>>|<cell|<with|math-font-family|rm|G5>>|<cell|0>|<cell|-<with|math-font-family|rm|G3>>>|<row|<cell|0>|<cell|<sqrt|2>*<with|math-font-family|rm|G5>>|<cell|-<sqrt|2>*<with|math-font-family|rm|G5>>|<cell|<with|math-font-family|rm|G4>>|<cell|<with|math-font-family|rm|G3>>|<cell|0>>>>>>>
      </unfolded-io>

      <\input>
        <with|color|red|(<with|math-font-family|rm|%i>13) >
      <|input>
        \;
      </input>
    </input>

    \;
  </session>
</body>

<\initial>
  <\collection>
    <associate|page-type|a2>
  </collection>
</initial>