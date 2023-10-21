# klipper-16x4-display-design-tool

![image](https://github.com/jrymk/klipper-16x4-display-design-tool/assets/39593345/5341fd8d-50a6-4c57-a914-88160bff46b8)

- Uses default bitmap font pulled straight from Klipper repo
- Align icons with text and get the proportions right easier
- Draw glyphs visually on a 1x1 grid instead of using `.` and `*` one pixel at a time on a text editor.
- Quickly export a glyph or the whole screen as a display group for doing weird stuff

---

*Disclaimer: Code poorly written, just drafted it out as fast as I could to get my V0 display customization journey going. It also doesn't do much. All string IO is done on the standard IO stream.*


---

Press G and then go to the terminal and paste the following to insert a glyph. Same for T key for inserting text.
```
Insert glyph: (16x16)
................
  ................
  ..************..
  .....******.....
  ..************..
  .....******.....
  ..********...*..
  ...........*....
  ....***....**...
  ....***.******..
  ....***.*******.
  ........******..
  ......*....**...
  .......**..*....
  ................
  ................
```
