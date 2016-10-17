<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
  
  


  <head>
    <title>
      bcast_test_toy_segv.cpp on Ticket #2350 – Attachment
     – mpich
    </title>
      <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
      <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <!--[if IE]><script type="text/javascript">
      if (/^#__msie303:/.test(window.location.hash))
        window.location.replace(window.location.hash.replace(/^#__msie303:/, '#'));
    </script><![endif]-->
        <link rel="search" href="/projects/mpich/search" />
        <link rel="help" href="/projects/mpich/wiki/TracGuide" />
        <link rel="alternate" href="/projects/mpich/raw-attachment/ticket/2350/bcast_test_toy_segv.cpp" type="text/x-c++src; charset=utf-8" title="Original Format" />
        <link rel="up" href="/projects/mpich/ticket/2350" title="Ticket #2350" />
        <link rel="start" href="/projects/mpich/wiki" />
        <link rel="stylesheet" href="/projects/mpich/chrome/common/css/trac.css" type="text/css" /><link rel="stylesheet" href="/projects/mpich/chrome/common/css/code.css" type="text/css" />
        <link rel="shortcut icon" href="/projects/mpich/chrome/common/trac.ico" type="image/x-icon" />
        <link rel="icon" href="/projects/mpich/chrome/common/trac.ico" type="image/x-icon" />
      <link type="application/opensearchdescription+xml" rel="search" href="/projects/mpich/search/opensearch" title="Search mpich" />
      <script type="text/javascript" charset="utf-8" src="/projects/mpich/chrome/common/js/jquery.js"></script>
      <script type="text/javascript" charset="utf-8" src="/projects/mpich/chrome/common/js/babel.js"></script>
      <script type="text/javascript" charset="utf-8" src="/projects/mpich/chrome/common/js/trac.js"></script>
      <script type="text/javascript" charset="utf-8" src="/projects/mpich/chrome/common/js/search.js"></script>
      <script type="text/javascript" src="/projects/mpich/chrome/common/js/folding.js"></script>
      <script type="text/javascript">
        jQuery(document).ready(function($) {
          $('#preview table.code').enableCollapsibleColumns($('#preview table.code thead th.content'));
        });
      </script>
  </head>
  <body>
    <div id="banner">
      <div id="header">
        <a id="logo" href="http://trac.mpich.org/projects/mpich/"><img src="/projects/mpich/chrome/common/trac_banner.png" alt="" /></a>
      </div>
      <form id="search" action="/projects/mpich/search" method="get">
        <div>
          <label for="proj-search">Search:</label>
          <input type="text" id="proj-search" name="q" size="18" value="" />
          <input type="submit" value="Search" />
        </div>
      </form>
      <div id="metanav" class="nav">
    <ul>
      <li class="first"><a href="/projects/mpich/login">Login</a></li><li><a href="/projects/mpich/prefs">Preferences</a></li><li><a href="/projects/mpich/wiki/TracGuide">Help/Guide</a></li><li class="last"><a href="/projects/mpich/about">About Trac</a></li>
    </ul>
  </div>
    </div>
    <div id="mainnav" class="nav">
    <ul>
      <li class="first"><a href="/projects/mpich/wiki">Wiki</a></li><li><a href="/projects/mpich/timeline">Timeline</a></li><li><a href="/projects/mpich/roadmap">Roadmap</a></li><li><a href="/projects/mpich/browser">Browse Source</a></li><li><a href="/projects/mpich/report">View Tickets</a></li><li class="last"><a href="/projects/mpich/search">Search</a></li>
    </ul>
  </div>
    <div id="main">
      <div id="ctxtnav" class="nav">
        <h2>Context Navigation</h2>
        <ul>
          <li class="last first"><a href="/projects/mpich/ticket/2350">Back to Ticket #2350</a></li>
        </ul>
        <hr />
      </div>
    <div id="content" class="attachment">
        <h1><a href="/projects/mpich/ticket/2350">Ticket #2350</a>: bcast_test_toy_segv.cpp</h1>
        <table id="info" summary="Description">
          <tbody>
            <tr>
              <th scope="col">File bcast_test_toy_segv.cpp,
                <span title="4130 bytes">4.0 KB</span>
                (added by aamer, <a class="timeline" href="/projects/mpich/timeline?from=2016-09-07T17%3A15%3A38-05%3A00&amp;precision=second" title="See timeline at 09/07/16 17:15:38">6 weeks ago</a>)</th>
            </tr>
            <tr>
              <td class="message searchable">
                
              </td>
            </tr>
          </tbody>
        </table>
        <div id="preview" class="searchable">
          
  <table class="code"><thead><tr><th class="lineno" title="Line numbers">Line</th><th class="content"> </th></tr></thead><tbody><tr><th id="L1"><a href="#L1">1</a></th><td>/*</td></tr><tr><th id="L2"><a href="#L2">2</a></th><td> * Build with something like:</td></tr><tr><th id="L3"><a href="#L3">3</a></th><td> *   mpic++ -ggdb -fno-inline -O0 -fno-omit-frame-pointer -std=c++11 -o bcast_test bcast_test_toy_segv.cpp</td></tr><tr><th id="L4"><a href="#L4">4</a></th><td> * </td></tr><tr><th id="L5"><a href="#L5">5</a></th><td> * ####################################</td></tr><tr><th id="L6"><a href="#L6">6</a></th><td> * Run with something like:</td></tr><tr><th id="L7"><a href="#L7">7</a></th><td> *   mpirun -np 6 bcast_test 400 70</td></tr><tr><th id="L8"><a href="#L8">8</a></th><td> */</td></tr><tr><th id="L9"><a href="#L9">9</a></th><td></td></tr><tr><th id="L10"><a href="#L10">10</a></th><td>#include "assert.h"</td></tr><tr><th id="L11"><a href="#L11">11</a></th><td>#include &lt;cstdlib&gt;</td></tr><tr><th id="L12"><a href="#L12">12</a></th><td>#include &lt;cstring&gt;</td></tr><tr><th id="L13"><a href="#L13">13</a></th><td>#include &lt;iomanip&gt;</td></tr><tr><th id="L14"><a href="#L14">14</a></th><td>#include &lt;iostream&gt;</td></tr><tr><th id="L15"><a href="#L15">15</a></th><td>#include &lt;mpi.h&gt;</td></tr><tr><th id="L16"><a href="#L16">16</a></th><td>#include &lt;string&gt;</td></tr><tr><th id="L17"><a href="#L17">17</a></th><td></td></tr><tr><th id="L18"><a href="#L18">18</a></th><td>enum class PayloadValueMode { POSITIONAL, SMALL, TINY };</td></tr><tr><th id="L19"><a href="#L19">19</a></th><td></td></tr><tr><th id="L20"><a href="#L20">20</a></th><td>template &lt;typename PayloadType&gt;</td></tr><tr><th id="L21"><a href="#L21">21</a></th><td>inline PayloadType payload_value(int iteration, int sender_rank, int position,</td></tr><tr><th id="L22"><a href="#L22">22</a></th><td>                                 PayloadValueMode mode = PayloadValueMode::POSITIONAL) noexcept {</td></tr><tr><th id="L23"><a href="#L23">23</a></th><td></td></tr><tr><th id="L24"><a href="#L24">24</a></th><td>    PayloadType ret_val;</td></tr><tr><th id="L25"><a href="#L25">25</a></th><td>    if (std::is_floating_point&lt;PayloadType&gt;::value) {</td></tr><tr><th id="L26"><a href="#L26">26</a></th><td>        ret_val = (((iteration) + 1) * 1.00000001 + (sender_rank)*1.001001 + (position)*1.000001);</td></tr><tr><th id="L27"><a href="#L27">27</a></th><td>    } else if (std::is_integral&lt;PayloadType&gt;::value) {</td></tr><tr><th id="L28"><a href="#L28">28</a></th><td></td></tr><tr><th id="L29"><a href="#L29">29</a></th><td>        if (mode == PayloadValueMode::POSITIONAL) {</td></tr><tr><th id="L30"><a href="#L30">30</a></th><td>            // SCHEME WILL ENCODE USING the following:</td></tr><tr><th id="L31"><a href="#L31">31</a></th><td>            // &lt;iteration+1&gt;&lt;00000&gt;&lt;sender_rank&gt;&lt;00&gt;&lt;position i&gt;</td></tr><tr><th id="L32"><a href="#L32">32</a></th><td>            ret_val = (((iteration) + 1) * 100000 + (sender_rank)*100 + (position)*1);</td></tr><tr><th id="L33"><a href="#L33">33</a></th><td>        } else if (mode == PayloadValueMode::SMALL) {</td></tr><tr><th id="L34"><a href="#L34">34</a></th><td>            ret_val = (((iteration) + 1) * 8 + (sender_rank)*4 + (position)*1);</td></tr><tr><th id="L35"><a href="#L35">35</a></th><td>        } else if (mode == PayloadValueMode::TINY) {</td></tr><tr><th id="L36"><a href="#L36">36</a></th><td>            ret_val = (((iteration) + 1) * 1.3 + (sender_rank)*1 + (position)*1);</td></tr><tr><th id="L37"><a href="#L37">37</a></th><td>        } else {</td></tr><tr><th id="L38"><a href="#L38">38</a></th><td>            fprintf(stderr, "Invalid mode %d\n", mode);</td></tr><tr><th id="L39"><a href="#L39">39</a></th><td>            exit(EXIT_FAILURE);</td></tr><tr><th id="L40"><a href="#L40">40</a></th><td>        }</td></tr><tr><th id="L41"><a href="#L41">41</a></th><td>    } else {</td></tr><tr><th id="L42"><a href="#L42">42</a></th><td>        fprintf(stderr, "Unsupported PayloadType. Unable to generated payload_value for types other than "</td></tr><tr><th id="L43"><a href="#L43">43</a></th><td>                 "floating point and integral values\n");</td></tr><tr><th id="L44"><a href="#L44">44</a></th><td>        exit(EXIT_FAILURE);</td></tr><tr><th id="L45"><a href="#L45">45</a></th><td>    }</td></tr><tr><th id="L46"><a href="#L46">46</a></th><td></td></tr><tr><th id="L47"><a href="#L47">47</a></th><td>    return ret_val;</td></tr><tr><th id="L48"><a href="#L48">48</a></th><td>}</td></tr><tr><th id="L49"><a href="#L49">49</a></th><td></td></tr><tr><th id="L50"><a href="#L50">50</a></th><td>template &lt;typename PayloadType&gt;</td></tr><tr><th id="L51"><a href="#L51">51</a></th><td>int run_test(int my_rank, int num_ranks, int count, int root_rank, const MPI_Datatype&amp; datatype,</td></tr><tr><th id="L52"><a href="#L52">52</a></th><td>             size_t iterations) {</td></tr><tr><th id="L53"><a href="#L53">53</a></th><td></td></tr><tr><th id="L54"><a href="#L54">54</a></th><td>    assert(my_rank &lt; num_ranks);</td></tr><tr><th id="L55"><a href="#L55">55</a></th><td></td></tr><tr><th id="L56"><a href="#L56">56</a></th><td>    // step 1. create arrays.</td></tr><tr><th id="L57"><a href="#L57">57</a></th><td>    PayloadType* buf = new PayloadType[count];</td></tr><tr><th id="L58"><a href="#L58">58</a></th><td>    PayloadType* check_buf = new PayloadType[count];</td></tr><tr><th id="L59"><a href="#L59">59</a></th><td></td></tr><tr><th id="L60"><a href="#L60">60</a></th><td>    bool             is_root      = (my_rank == root_rank);</td></tr><tr><th id="L61"><a href="#L61">61</a></th><td>    PayloadValueMode payload_mode = PayloadValueMode::TINY;</td></tr><tr><th id="L62"><a href="#L62">62</a></th><td>    int              has_error    = 0;</td></tr><tr><th id="L63"><a href="#L63">63</a></th><td></td></tr><tr><th id="L64"><a href="#L64">64</a></th><td>    ///////////////////////////////////////////////////////////////////////////</td></tr><tr><th id="L65"><a href="#L65">65</a></th><td></td></tr><tr><th id="L66"><a href="#L66">66</a></th><td>    // Step 3a. Initialize buf values (for root) and check_buf (for all ranks)</td></tr><tr><th id="L67"><a href="#L67">67</a></th><td>    for (auto l = 0; l &lt; iterations; ++l) {</td></tr><tr><th id="L68"><a href="#L68">68</a></th><td>        for (auto i = 0; i &lt; count; ++i) {</td></tr><tr><th id="L69"><a href="#L69">69</a></th><td>            PayloadType v = payload_value&lt;PayloadType&gt;(l, root_rank, i, payload_mode);</td></tr><tr><th id="L70"><a href="#L70">70</a></th><td>            if (is_root) {</td></tr><tr><th id="L71"><a href="#L71">71</a></th><td>                buf[i] = v;</td></tr><tr><th id="L72"><a href="#L72">72</a></th><td>            }</td></tr><tr><th id="L73"><a href="#L73">73</a></th><td>            check_buf[i] = v;</td></tr><tr><th id="L74"><a href="#L74">74</a></th><td>        } // for count</td></tr><tr><th id="L75"><a href="#L75">75</a></th><td></td></tr><tr><th id="L76"><a href="#L76">76</a></th><td>        // Step 3b. Execute broadcast</td></tr><tr><th id="L77"><a href="#L77">77</a></th><td>        MPI::COMM_WORLD.Bcast(buf, count, datatype, root_rank);</td></tr><tr><th id="L78"><a href="#L78">78</a></th><td></td></tr><tr><th id="L79"><a href="#L79">79</a></th><td>        // Step 4. Verify correct received results of the bcast channel</td></tr><tr><th id="L80"><a href="#L80">80</a></th><td>        for (auto i = 0; i &lt; count; ++i) {</td></tr><tr><th id="L81"><a href="#L81">81</a></th><td>            assert(buf[i] == check_buf[i]);</td></tr><tr><th id="L82"><a href="#L82">82</a></th><td>        }</td></tr><tr><th id="L83"><a href="#L83">83</a></th><td>    } // for iterations</td></tr><tr><th id="L84"><a href="#L84">84</a></th><td></td></tr><tr><th id="L85"><a href="#L85">85</a></th><td>    // step 5. clean up</td></tr><tr><th id="L86"><a href="#L86">86</a></th><td>    delete[](buf);</td></tr><tr><th id="L87"><a href="#L87">87</a></th><td>    delete[](check_buf);</td></tr><tr><th id="L88"><a href="#L88">88</a></th><td></td></tr><tr><th id="L89"><a href="#L89">89</a></th><td>    return has_error;</td></tr><tr><th id="L90"><a href="#L90">90</a></th><td>} // function run_test</td></tr><tr><th id="L91"><a href="#L91">91</a></th><td></td></tr><tr><th id="L92"><a href="#L92">92</a></th><td>int main(int argc, char* argv[]) {</td></tr><tr><th id="L93"><a href="#L93">93</a></th><td></td></tr><tr><th id="L94"><a href="#L94">94</a></th><td>    int iterations = -1;</td></tr><tr><th id="L95"><a href="#L95">95</a></th><td>    int count      = -1;</td></tr><tr><th id="L96"><a href="#L96">96</a></th><td></td></tr><tr><th id="L97"><a href="#L97">97</a></th><td>    if (argc &lt; 3) {</td></tr><tr><th id="L98"><a href="#L98">98</a></th><td>        fprintf(stderr, "ERROR: usage: %s &lt;num_iterations&gt; &lt;count&gt;", argv[0]);</td></tr><tr><th id="L99"><a href="#L99">99</a></th><td>        exit(-1);</td></tr><tr><th id="L100"><a href="#L100">100</a></th><td>    } else {</td></tr><tr><th id="L101"><a href="#L101">101</a></th><td>        iterations = atoi(argv[1]);</td></tr><tr><th id="L102"><a href="#L102">102</a></th><td>        assert(iterations &gt; 0);</td></tr><tr><th id="L103"><a href="#L103">103</a></th><td>        count = atoi(argv[2]);</td></tr><tr><th id="L104"><a href="#L104">104</a></th><td>        assert(count &gt; 0);</td></tr><tr><th id="L105"><a href="#L105">105</a></th><td>        printf("Running with %d bcast iteration(s) and count %d\n", iterations, count);</td></tr><tr><th id="L106"><a href="#L106">106</a></th><td>    }</td></tr><tr><th id="L107"><a href="#L107">107</a></th><td></td></tr><tr><th id="L108"><a href="#L108">108</a></th><td>    MPI::Init(argc, argv);</td></tr><tr><th id="L109"><a href="#L109">109</a></th><td></td></tr><tr><th id="L110"><a href="#L110">110</a></th><td>    int my_rank   = MPI::COMM_WORLD.Get_rank();</td></tr><tr><th id="L111"><a href="#L111">111</a></th><td>    int num_ranks = MPI::COMM_WORLD.Get_size();</td></tr><tr><th id="L112"><a href="#L112">112</a></th><td></td></tr><tr><th id="L113"><a href="#L113">113</a></th><td>    int num_errors = 0;</td></tr><tr><th id="L114"><a href="#L114">114</a></th><td></td></tr><tr><th id="L115"><a href="#L115">115</a></th><td>    for (int root_rank = 0; root_rank &lt; num_ranks; ++root_rank) {</td></tr><tr><th id="L116"><a href="#L116">116</a></th><td>        num_errors += run_test&lt;int&gt;   (my_rank, num_ranks, count, root_rank, MPI_INT,    iterations);</td></tr><tr><th id="L117"><a href="#L117">117</a></th><td>        num_errors += run_test&lt;double&gt;(my_rank, num_ranks, count, root_rank, MPI_DOUBLE, iterations);</td></tr><tr><th id="L118"><a href="#L118">118</a></th><td>        num_errors += run_test&lt;float&gt; (my_rank, num_ranks, count, root_rank, MPI_FLOAT,  iterations);</td></tr><tr><th id="L119"><a href="#L119">119</a></th><td>    }</td></tr><tr><th id="L120"><a href="#L120">120</a></th><td></td></tr><tr><th id="L121"><a href="#L121">121</a></th><td>    MPI::Finalize();</td></tr><tr><th id="L122"><a href="#L122">122</a></th><td></td></tr><tr><th id="L123"><a href="#L123">123</a></th><td>    return 0;</td></tr><tr><th id="L124"><a href="#L124">124</a></th><td>}</td></tr></tbody></table>

        </div>
    </div>
    <div id="altlinks">
      <h3>Download in other formats:</h3>
      <ul>
        <li class="last first">
          <a rel="nofollow" href="/projects/mpich/raw-attachment/ticket/2350/bcast_test_toy_segv.cpp">Original Format</a>
        </li>
      </ul>
    </div>
    </div>
    <div id="footer" lang="en" xml:lang="en"><hr />
      <a id="tracpowered" href="http://trac.edgewall.org/"><img src="/projects/mpich/chrome/common/trac_logo_mini.png" height="30" width="107" alt="Trac Powered" /></a>
      <p class="left">Powered by <a href="/projects/mpich/about"><strong>Trac 1.0</strong></a><br />
        By <a href="http://www.edgewall.org/">Edgewall Software</a>.</p>
      <p class="right">Visit the Trac open source project at<br /><a href="http://trac.edgewall.org/">http://trac.edgewall.org/</a></p>
    </div>
  </body>
</html>