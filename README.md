# The Giblet Initiative

The Giblet Initiative is an effort to resurrect the classic version of Yahoo! Messenger for the desktop. This includes not only the Messenger client itself but services such as voice, webcam, and file transfers to support it. Existing protocols such as XMPP and Facebook's Messenger API are targeted for connectivity with additional web services providing content when available.

The project is built using modern C++ and Visual Studio 2017. Currently the only way to launch the project is through the test harness. This requires a modification to the hosts file to handle some of the connection management until a proper solution can be implemented. A sample hosts file is provided in TestHarness/etc.
