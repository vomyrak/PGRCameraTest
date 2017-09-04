# PGRCameraTest

lightstage control interface in development
created by Husheng Deng on 30 Jun 17

Philips kinet control api is
Based on kinet.h created by Michael Dewberry on 12/19/12.
Port of kinet.py by Giles Hall
http://github.com/vishnubob/kinet

GUI working in progress

## Important Update
- The order of the arguments taken by functions that set light values have been changed
From (r, g, b, r2, g2, b2) to (r, r2, g, g2, b, b2) to standardise with the protocol
Used by the power units.

- This change in addition to the changes made to internal indexing helps to avoid switching
Between two different ways of indexing light values that existed in previous code
