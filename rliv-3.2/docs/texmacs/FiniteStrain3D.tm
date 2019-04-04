<TeXmacs|1.0.7.15>

<style|<tuple|generic|maxima>>

<\body>
  Compute deformation tensors from the\ 

  <\session|maxima|default>
    <\output>
      Maxima 5.27.0 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) >
    <|unfolded-io>
      F:matrix([F0,F3,F5],[F4,F1,F7],[F6,F8,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>22) >
    <|unfolded-io>
      transpose(F).F /* right cauchy green tensor */
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o22>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F0><rsup|2>>|<cell|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1><rsup|2>>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>23) >
    <|unfolded-io>
      F.transpose(F) /* left cauchy green tensor */
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o23>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0><rsup|2>>|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F1><rsup|2>>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      p:matrix([p0,p3/sqrt(2),p4/sqrt(2)],[p3/sqrt(2),p1,p5/sqrt(2)],[p4/sqrt(2),p5/sqrt(2),p2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|p0>>|<cell|<frac|<with|math-font-family|rm|p3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|p4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|p3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|p1>>|<cell|<frac|<with|math-font-family|rm|p5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|p4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|p5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|p2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>25) >
    <|unfolded-io>
      s:F.p.transpose(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o25>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>+<with|math-font-family|rm|F0>*<around*|(|<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F5>|<sqrt|2>>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F5>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>|<sqrt|2>>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>|<sqrt|2>>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F4>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F4>|<sqrt|2>>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>>|<cell|<with|math-font-family|rm|F5>*<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>|<sqrt|2>>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>|<sqrt|2>>+<with|math-font-family|rm|p2>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F0>*<around*|(|<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F8>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F6>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|<sqrt|2>>|)>+<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p1>*<with|math-font-family|rm|F8>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F6>|<sqrt|2>>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F2>|<sqrt|2>>|)>>>|<row|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F5>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>|<sqrt|2>>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F1>*<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>+<with|math-font-family|rm|F4>*<around*|(|<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F5>|<sqrt|2>>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|F1>*<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>|<sqrt|2>>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F4>*<around*|(|<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F4>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F4>|<sqrt|2>>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>>|<cell|<with|math-font-family|rm|F7>*<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>|<sqrt|2>>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>|<sqrt|2>>+<with|math-font-family|rm|p2>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F4>*<around*|(|<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F8>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F6>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|<sqrt|2>>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p1>*<with|math-font-family|rm|F8>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F6>|<sqrt|2>>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F2>|<sqrt|2>>|)>>>|<row|<cell|<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>*<with|math-font-family|rm|F8>+<around*|(|<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F5>|<sqrt|2>>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F5>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>|<sqrt|2>>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>>|<cell|<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>|<sqrt|2>>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F6>*<around*|(|<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F4>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F4>|<sqrt|2>>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>>|<cell|<with|math-font-family|rm|F2>*<around*|(|<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>|<sqrt|2>>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>|<sqrt|2>>+<with|math-font-family|rm|p2>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F6>*<around*|(|<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F8>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F6>+<frac|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|<sqrt|2>>|)>+<with|math-font-family|rm|F8>*<around*|(|<with|math-font-family|rm|p1>*<with|math-font-family|rm|F8>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F6>|<sqrt|2>>+<frac|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F2>|<sqrt|2>>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>26) >
    <|unfolded-io>
      fullratsimp(s[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o26>)
      >><with|math-font-family|rm|p2>*<with|math-font-family|rm|F5><rsup|2>+<around*|(|<sqrt|2>*<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<sqrt|2>*<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3><rsup|2>+<sqrt|2>*<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0><rsup|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>27) >
    <|unfolded-io>
      fullratsimp(s[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o27>)
      >><with|math-font-family|rm|p2>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|<sqrt|2>*<with|math-font-family|rm|p4>*<with|math-font-family|rm|F4>+<sqrt|2>*<with|math-font-family|rm|p5>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F4><rsup|2>+<sqrt|2>*<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1><rsup|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>28) >
    <|unfolded-io>
      fullratsimp(s[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o28>)
      >><with|math-font-family|rm|p1>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|<sqrt|2>*<with|math-font-family|rm|p3>*<with|math-font-family|rm|F6>+<sqrt|2>*<with|math-font-family|rm|p5>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F6><rsup|2>+<sqrt|2>*<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p2>*<with|math-font-family|rm|F2><rsup|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      fullratsimp(s[1,2]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><around*|(|<sqrt|2>*<with|math-font-family|rm|p2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F7>+<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F5>+<around*|(|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<sqrt|2>*<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F4>+<sqrt|2>*<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|unfolded-io>
      fullratsimp(s[1,3]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o30>)
      >><around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<sqrt|2>*<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F8>+<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<sqrt|2>*<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F6>+<sqrt|2>*<with|math-font-family|rm|p2>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>31) >
    <|unfolded-io>
      fullratsimp(s[2,3]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o31>)
      >><around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<sqrt|2>*<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F8>+<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<sqrt|2>*<with|math-font-family|rm|p2>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|F7>+<around*|(|<sqrt|2>*<with|math-font-family|rm|p0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>32) >
    <|unfolded-io>
      e:0.5*(transpose(F).F-ident(3))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o32>)
      >><matrix|<tformat|<table|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>|)>>>|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1><rsup|2>-1|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|)>>>|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F2><rsup|2>-1|)>>>>>>>>
    </unfolded-io>

    <\folded-io>
      <with|color|red|(<with|math-font-family|rm|%i>34) >
    <|folded-io>
      fullratsimp(F.e.transpose(F))
    <|folded-io>
      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      \;

      rat: replaced 0.5 by 1/2 = 0.5

      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o34>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|F3><rsup|2>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>+2*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5><rsup|2>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0><rsup|2>*<with|math-font-family|rm|F6><rsup|2>+2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F5><rsup|4>+<around*|(|2*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F2><rsup|2>+2*<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F0><rsup|2>*<with|math-font-family|rm|F4><rsup|2>+2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F3><rsup|4>+<around*|(|<with|math-font-family|rm|F1><rsup|2>+2*<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0><rsup|4>-<with|math-font-family|rm|F0><rsup|2>|2>>|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F7>+<around*|(|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7><rsup|3>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F5><rsup|3>+<around*|(|<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4><rsup|3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4><rsup|2>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|3>-<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3><rsup|3>+<around*|(|<with|math-font-family|rm|F1><rsup|3>+<around*|(|<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F3>|2>>|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8><rsup|3>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F3><rsup|3>+<around*|(|<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6><rsup|3>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6><rsup|2>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F0><rsup|3>-<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5><rsup|3>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F2><rsup|3>+<around*|(|<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|F5>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F7>+<around*|(|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7><rsup|3>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F5><rsup|3>+<around*|(|<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4><rsup|3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4><rsup|2>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|3>-<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3><rsup|3>+<around*|(|<with|math-font-family|rm|F1><rsup|3>+<around*|(|<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F3>|2>>|<cell|<frac|<with|math-font-family|rm|F1><rsup|2>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F7><rsup|4>+<around*|(|<with|math-font-family|rm|F5><rsup|2>+2*<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F2><rsup|2>+2*<with|math-font-family|rm|F1><rsup|2>-1|)>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|2*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>+<around*|(|2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4><rsup|2>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F4><rsup|4>+<around*|(|2*<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F4><rsup|2>+2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1><rsup|2>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1><rsup|4>-<with|math-font-family|rm|F1><rsup|2>|2>>|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8><rsup|3>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>|)>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|3>-<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7><rsup|3>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F2><rsup|3>+<around*|(|<with|math-font-family|rm|F1><rsup|2>-1|)>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6><rsup|3>+<around*|(|<with|math-font-family|rm|F4><rsup|3>+<around*|(|<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F6>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F5>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8><rsup|3>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F3><rsup|3>+<around*|(|<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6><rsup|3>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6><rsup|2>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F0><rsup|3>-<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5><rsup|3>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F2><rsup|3>+<around*|(|<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|F5>|2>>|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8><rsup|3>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>|)>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|3>-<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7><rsup|3>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>*<with|math-font-family|rm|F7><rsup|2>+<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F2><rsup|3>+<around*|(|<with|math-font-family|rm|F1><rsup|2>-1|)>*<with|math-font-family|rm|F2>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6><rsup|3>+<around*|(|<with|math-font-family|rm|F4><rsup|3>+<around*|(|<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F6>+<around*|(|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F5>|2>>|<cell|<frac|<with|math-font-family|rm|F8><rsup|4>+<around*|(|2*<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+2*<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>-1|)>*<with|math-font-family|rm|F8><rsup|2>+<around*|(|2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>+<around*|(|2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>*<with|math-font-family|rm|F6>+2*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F2><rsup|2>*<with|math-font-family|rm|F7><rsup|2>+2*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F6><rsup|4>+<around*|(|<with|math-font-family|rm|F4><rsup|2>+2*<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>*<with|math-font-family|rm|F6><rsup|2>+2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F2><rsup|2>*<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F2><rsup|4>-<with|math-font-family|rm|F2><rsup|2>|2>>>>>>>>
    </folded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>35) >
    <|input>
      \;
    </input>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>34) >
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
    <associate|page-orientation|portrait>
    <associate|page-type|a3>
  </collection>
</initial>