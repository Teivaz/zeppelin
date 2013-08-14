<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="53" name="tPadExt" color="7" fill="1" visible="no" active="no"/>
<layer number="54" name="bPadExt" color="1" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="100" name="Muster" color="7" fill="1" visible="no" active="no"/>
<layer number="250" name="Descript" color="3" fill="1" visible="no" active="no"/>
<layer number="251" name="SMDround" color="12" fill="11" visible="no" active="no"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="pinhead">
<description>&lt;b&gt;Pin Header Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="1X02">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-1.905" y1="1.27" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-2.6162" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
</package>
<package name="1X02/90">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-2.54" y1="-1.905" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="6.985" x2="-1.27" y2="1.27" width="0.762" layer="21"/>
<wire x1="0" y1="-1.905" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="6.985" x2="1.27" y2="1.27" width="0.762" layer="21"/>
<pad name="1" x="-1.27" y="-3.81" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="1.27" y="-3.81" drill="1.016" shape="long" rot="R90"/>
<text x="-3.175" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="4.445" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.651" y1="0.635" x2="-0.889" y2="1.143" layer="21"/>
<rectangle x1="0.889" y1="0.635" x2="1.651" y2="1.143" layer="21"/>
<rectangle x1="-1.651" y1="-2.921" x2="-0.889" y2="-1.905" layer="21"/>
<rectangle x1="0.889" y1="-2.921" x2="1.651" y2="-1.905" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="PINHD2">
<wire x1="-6.35" y1="-2.54" x2="1.27" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="1.27" y2="5.08" width="0.4064" layer="94"/>
<wire x1="1.27" y1="5.08" x2="-6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-6.35" y2="-2.54" width="0.4064" layer="94"/>
<text x="-6.35" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PINHD-1X2" prefix="JP" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="PINHD2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1X02">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="/90" package="1X02/90">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="irf-2">
<description>&lt;h1&gt;irf.lbr&lt;/h1&gt;

&lt;h2&gt;Description&lt;/h2&gt;
&lt;p&gt;This library contains some devices produced by International Rectifier:  HEXFET Power MOSFETs,  IGBTs.
&lt;/p&gt;

&lt;h2&gt;Authors&lt;/h2&gt;
&lt;p&gt;&lt;ul&gt;
  &lt;li&gt;Ladislav Kopecky: Initial Release&lt;/li&gt;
  &lt;li&gt;Tilo Renz&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;

&lt;h2&gt;Copyright &amp; License&lt;/h2&gt;
&lt;p&gt;This library is licensed under the General Public License, Version 2. &lt;br /&gt;
More Information about the General Public License is available at &lt;a href="http://www.gnu.org/copyleft/gpl.html"&gt;http://www.gnu.org/copyleft/gpl.html&lt;/a&gt;. &lt;br /&gt;
In short: You are welcome to use (and extend) this library, but extensions must be published under the same license.
&lt;/p&gt;

&lt;h2&gt;Version History&lt;/h2&gt;
&lt;dl&gt;
  &lt;dt&gt;2004-09-15&lt;/dt&gt;
  &lt;dd&gt;Initial Release&lt;/dd&gt;
  &lt;dt&gt;2005-03-21, v0.2&lt;/dt&gt;
  &lt;dd&gt;Documentation, IRF7103, IRF7104&lt;/dd&gt;
&lt;/dl&gt;

&lt;h2&gt;Additional Documentation&lt;/h2&gt;
&lt;p&gt;&lt;ul&gt;
  &lt;li&gt;The International Rectifier Homepage (&lt;a href="http://www.irf.com"&gt;http://www.irf.com&lt;/a&gt;)&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;</description>
<packages>
<package name="SO8-2">
<wire x1="-3.302" y1="0.127" x2="-3.302" y2="-1.27" width="0.127" layer="21"/>
<wire x1="-3.302" y1="0.127" x2="-3.302" y2="1.397" width="0.127" layer="21" curve="180"/>
<wire x1="2.032" y1="-1.27" x2="2.032" y2="2.54" width="0.127" layer="21"/>
<wire x1="2.032" y1="-1.27" x2="-3.302" y2="-1.27" width="0.127" layer="21"/>
<wire x1="-3.302" y1="2.54" x2="-3.302" y2="1.397" width="0.127" layer="21"/>
<wire x1="-3.302" y1="2.54" x2="2.032" y2="2.54" width="0.127" layer="21"/>
<smd name="1" x="-2.54" y="-2.54" dx="0.6096" dy="1.6256" layer="1"/>
<smd name="2" x="-1.27" y="-2.54" dx="0.6096" dy="1.6256" layer="1"/>
<smd name="3" x="0" y="-2.54" dx="0.6096" dy="1.6256" layer="1"/>
<smd name="4" x="1.27" y="-2.54" dx="0.6096" dy="1.6256" layer="1"/>
<smd name="5" x="1.27" y="3.81" dx="0.6096" dy="1.6256" layer="1"/>
<smd name="6" x="0" y="3.81" dx="0.6096" dy="1.6256" layer="1"/>
<smd name="7" x="-1.27" y="3.81" dx="0.6096" dy="1.6256" layer="1"/>
<smd name="8" x="-2.54" y="3.81" dx="0.6096" dy="1.6256" layer="1"/>
<text x="2.286" y="1.3208" size="1.27" layer="25" font="vector" ratio="10">&gt;NAME</text>
<text x="-2.5908" y="-0.9398" size="0.762" layer="21" font="vector" ratio="12">1</text>
<text x="2.286" y="-1.905" size="1.27" layer="27" font="vector" ratio="10">&gt;VALUE</text>
<text x="-2.5908" y="1.4732" size="0.762" layer="21" font="vector" ratio="12">8</text>
<text x="0.5588" y="-0.9398" size="0.762" layer="21" font="vector" ratio="12">4</text>
<text x="0.6858" y="1.4732" size="0.762" layer="21" font="vector" ratio="12">5</text>
</package>
</packages>
<symbols>
<symbol name="P-CHANNEL-MOSFET">
<wire x1="-1.143" y1="0.762" x2="-1.143" y2="-0.762" width="0.254" layer="94"/>
<wire x1="-1.143" y1="3.683" x2="-1.143" y2="1.397" width="0.254" layer="94"/>
<wire x1="-1.143" y1="-1.397" x2="-1.143" y2="-3.683" width="0.254" layer="94"/>
<wire x1="-2.286" y1="2.54" x2="-2.286" y2="-2.54" width="0.254" layer="94"/>
<wire x1="3.175" y1="-0.508" x2="1.905" y2="-0.508" width="0.1524" layer="94"/>
<wire x1="3.175" y1="-0.508" x2="3.429" y2="-0.254" width="0.1524" layer="94"/>
<wire x1="1.905" y1="-0.508" x2="1.651" y2="-0.762" width="0.1524" layer="94"/>
<wire x1="2.54" y1="2.54" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-1.016" y1="-2.54" x2="1.016" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="1.016" y1="-2.54" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="2.54" x2="-1.016" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-1.016" y1="0" x2="1.016" y2="0" width="0.1524" layer="94"/>
<wire x1="1.016" y1="0" x2="1.016" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-2.286" y1="-2.54" x2="-2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="2.54" x2="5.08" y2="2.54" width="0.1524" layer="94"/>
<circle x="0" y="0" radius="4.318" width="0.1524" layer="94"/>
<text x="-4.572" y="2.54" size="1.778" layer="96" rot="R180">&gt;VALUE</text>
<text x="-4.572" y="5.08" size="1.778" layer="95" rot="R180">&gt;NAME</text>
<pin name="G" x="-5.08" y="-2.54" visible="off" length="short"/>
<pin name="S" x="2.54" y="-5.08" visible="off" length="short" rot="R90"/>
<pin name="D@1" x="2.54" y="5.08" visible="off" length="short" swaplevel="1" rot="R270"/>
<pin name="D@2" x="5.08" y="5.08" visible="off" length="short" swaplevel="1" rot="R270"/>
<polygon width="0.1524" layer="94">
<vertex x="2.54" y="-0.381"/>
<vertex x="3.175" y="0.889"/>
<vertex x="1.905" y="0.889"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="0.889" y="0"/>
<vertex x="-0.381" y="-0.635"/>
<vertex x="-0.381" y="0.635"/>
</polygon>
</symbol>
<symbol name="N-CHANNEL-MOSFET">
<wire x1="-1.143" y1="0.762" x2="-1.143" y2="-0.762" width="0.254" layer="94"/>
<wire x1="-1.143" y1="3.683" x2="-1.143" y2="1.397" width="0.254" layer="94"/>
<wire x1="-1.143" y1="-1.397" x2="-1.143" y2="-3.683" width="0.254" layer="94"/>
<wire x1="-2.286" y1="2.54" x2="-2.286" y2="-2.54" width="0.254" layer="94"/>
<wire x1="1.905" y1="0.762" x2="3.175" y2="0.762" width="0.1524" layer="94"/>
<wire x1="1.905" y1="0.762" x2="1.651" y2="0.508" width="0.1524" layer="94"/>
<wire x1="3.175" y1="0.762" x2="3.429" y2="1.016" width="0.1524" layer="94"/>
<wire x1="2.54" y1="2.54" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-1.016" y1="-2.54" x2="1.016" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="1.016" y1="-2.54" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="2.54" x2="-1.016" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-1.016" y1="0" x2="1.016" y2="0" width="0.1524" layer="94"/>
<wire x1="1.016" y1="0" x2="1.016" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-2.286" y1="-2.54" x2="-2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="2.54" x2="5.08" y2="2.54" width="0.1524" layer="94"/>
<circle x="0" y="0" radius="4.318" width="0.1524" layer="94"/>
<text x="-4.572" y="2.54" size="1.778" layer="96" rot="R180">&gt;VALUE</text>
<text x="-4.572" y="5.08" size="1.778" layer="95" rot="R180">&gt;NAME</text>
<pin name="G" x="-5.08" y="-2.54" visible="off" length="short"/>
<pin name="S" x="2.54" y="-5.08" visible="off" length="short" rot="R90"/>
<pin name="D@1" x="2.54" y="5.08" visible="off" length="short" swaplevel="1" rot="R270"/>
<pin name="D@2" x="5.08" y="5.08" visible="off" length="short" swaplevel="1" rot="R270"/>
<polygon width="0.1524" layer="94">
<vertex x="2.54" y="0.635"/>
<vertex x="1.905" y="-0.635"/>
<vertex x="3.175" y="-0.635"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="-0.889" y="0"/>
<vertex x="0.381" y="0.635"/>
<vertex x="0.381" y="-0.635"/>
</polygon>
</symbol>
</symbols>
<devicesets>
<deviceset name="IRF7105">
<description>&lt;p&gt;HEXFET® Power MOSFET&lt;/p&gt;
&lt;ul&gt;
  &lt;li&gt;Adavanced Process Technology&lt;/li&gt;
  &lt;li&gt;Ultra Low On-Resistance&lt;/li&gt;
  &lt;li&gt;Dual N and P Channel MOSFET&lt;/li&gt;
  &lt;li&gt;Surface Mount&lt;/li&gt;
  &lt;li&gt;Available in Tape &amp; Reel&lt;/li&gt;
  &lt;li&gt;Dynamic dv/dt Rating&lt;/li&gt;
  &lt;li&gt;Fast Switching&lt;/li&gt;
&lt;/ul&gt;
&lt;b&gt;Please connect both Drain-Pins of each FET for thermal Reasons! (Pin 5 with Pin 6, 7 with 8)&lt;/b&gt;</description>
<gates>
<gate name="G$2" symbol="P-CHANNEL-MOSFET" x="0" y="-10.16" swaplevel="1"/>
<gate name="G$1" symbol="N-CHANNEL-MOSFET" x="0" y="7.62"/>
</gates>
<devices>
<device name="" package="SO8-2">
<connects>
<connect gate="G$1" pin="D@1" pad="7"/>
<connect gate="G$1" pin="D@2" pad="8"/>
<connect gate="G$1" pin="G" pad="2"/>
<connect gate="G$1" pin="S" pad="1"/>
<connect gate="G$2" pin="D@1" pad="6"/>
<connect gate="G$2" pin="D@2" pad="5"/>
<connect gate="G$2" pin="G" pad="4"/>
<connect gate="G$2" pin="S" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="INPUT" library="pinhead" deviceset="PINHD-1X2" device=""/>
<part name="DC_MOTOR" library="pinhead" deviceset="PINHD-1X2" device=""/>
<part name="U$1" library="irf-2" deviceset="IRF7105" device=""/>
<part name="U$2" library="irf-2" deviceset="IRF7105" device=""/>
<part name="POWER" library="pinhead" deviceset="PINHD-1X2" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="INPUT" gate="G$1" x="0" y="66.04" rot="R180"/>
<instance part="DC_MOTOR" gate="G$1" x="60.96" y="76.2" rot="R90"/>
<instance part="U$1" gate="G$2" x="40.64" y="76.2" rot="MR180"/>
<instance part="U$1" gate="G$1" x="40.64" y="55.88"/>
<instance part="U$2" gate="G$2" x="78.74" y="76.2" rot="R180"/>
<instance part="U$2" gate="G$1" x="78.74" y="55.88" rot="MR0"/>
<instance part="POWER" gate="G$1" x="10.16" y="93.98" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="G"/>
<wire x1="35.56" y1="53.34" x2="25.4" y2="53.34" width="0.1524" layer="91"/>
<wire x1="25.4" y1="53.34" x2="25.4" y2="66.04" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="G"/>
<wire x1="25.4" y1="66.04" x2="25.4" y2="78.74" width="0.1524" layer="91"/>
<wire x1="25.4" y1="78.74" x2="35.56" y2="78.74" width="0.1524" layer="91"/>
<pinref part="INPUT" gate="G$1" pin="2"/>
<wire x1="2.54" y1="66.04" x2="25.4" y2="66.04" width="0.1524" layer="91"/>
<junction x="25.4" y="66.04"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="U$2" gate="G$2" pin="G"/>
<wire x1="83.82" y1="78.74" x2="96.52" y2="78.74" width="0.1524" layer="91"/>
<wire x1="96.52" y1="78.74" x2="96.52" y2="53.34" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="G"/>
<wire x1="96.52" y1="53.34" x2="83.82" y2="53.34" width="0.1524" layer="91"/>
<pinref part="INPUT" gate="G$1" pin="1"/>
<wire x1="2.54" y1="63.5" x2="12.7" y2="63.5" width="0.1524" layer="91"/>
<wire x1="12.7" y1="63.5" x2="12.7" y2="40.64" width="0.1524" layer="91"/>
<wire x1="12.7" y1="40.64" x2="96.52" y2="40.64" width="0.1524" layer="91"/>
<wire x1="96.52" y1="40.64" x2="96.52" y2="53.34" width="0.1524" layer="91"/>
<junction x="96.52" y="53.34"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="D@1"/>
<pinref part="U$1" gate="G$2" pin="D@1"/>
<wire x1="43.18" y1="60.96" x2="43.18" y2="66.04" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="D@2"/>
<wire x1="43.18" y1="66.04" x2="43.18" y2="71.12" width="0.1524" layer="91"/>
<wire x1="43.18" y1="66.04" x2="45.72" y2="66.04" width="0.1524" layer="91"/>
<wire x1="45.72" y1="66.04" x2="45.72" y2="60.96" width="0.1524" layer="91"/>
<junction x="43.18" y="66.04"/>
<pinref part="U$1" gate="G$2" pin="D@2"/>
<wire x1="45.72" y1="71.12" x2="45.72" y2="66.04" width="0.1524" layer="91"/>
<junction x="45.72" y="66.04"/>
<pinref part="DC_MOTOR" gate="G$1" pin="1"/>
<wire x1="45.72" y1="66.04" x2="58.42" y2="66.04" width="0.1524" layer="91"/>
<wire x1="58.42" y1="66.04" x2="58.42" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="U$2" gate="G$2" pin="D@1"/>
<pinref part="U$2" gate="G$1" pin="D@1"/>
<wire x1="76.2" y1="71.12" x2="76.2" y2="66.04" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="D@2"/>
<pinref part="U$2" gate="G$2" pin="D@2"/>
<wire x1="76.2" y1="66.04" x2="76.2" y2="60.96" width="0.1524" layer="91"/>
<wire x1="73.66" y1="60.96" x2="73.66" y2="66.04" width="0.1524" layer="91"/>
<wire x1="73.66" y1="66.04" x2="73.66" y2="71.12" width="0.1524" layer="91"/>
<wire x1="76.2" y1="66.04" x2="73.66" y2="66.04" width="0.1524" layer="91"/>
<junction x="76.2" y="66.04"/>
<junction x="73.66" y="66.04"/>
<pinref part="DC_MOTOR" gate="G$1" pin="2"/>
<wire x1="73.66" y1="66.04" x2="60.96" y2="66.04" width="0.1524" layer="91"/>
<wire x1="60.96" y1="66.04" x2="60.96" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="POWER" gate="G$1" pin="2"/>
<pinref part="U$2" gate="G$2" pin="S"/>
<wire x1="12.7" y1="93.98" x2="43.18" y2="93.98" width="0.1524" layer="91"/>
<wire x1="43.18" y1="93.98" x2="76.2" y2="93.98" width="0.1524" layer="91"/>
<wire x1="76.2" y1="93.98" x2="76.2" y2="81.28" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="S"/>
<wire x1="43.18" y1="81.28" x2="43.18" y2="93.98" width="0.1524" layer="91"/>
<junction x="43.18" y="93.98"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="S"/>
<wire x1="43.18" y1="50.8" x2="43.18" y2="45.72" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="S"/>
<wire x1="43.18" y1="45.72" x2="76.2" y2="45.72" width="0.1524" layer="91"/>
<wire x1="76.2" y1="45.72" x2="76.2" y2="50.8" width="0.1524" layer="91"/>
<wire x1="43.18" y1="45.72" x2="20.32" y2="45.72" width="0.1524" layer="91"/>
<wire x1="20.32" y1="45.72" x2="20.32" y2="91.44" width="0.1524" layer="91"/>
<junction x="43.18" y="45.72"/>
<wire x1="20.32" y1="91.44" x2="12.7" y2="91.44" width="0.1524" layer="91"/>
<pinref part="POWER" gate="G$1" pin="1"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
