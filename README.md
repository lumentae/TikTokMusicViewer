# TikTok Music Viewer
With this program, you can view your saved TikTok music.
This program uses between 40MB to 50MB of RAM while playing music without autoplay.

---

## Limitations
- Due to problems with miniaudio, old sound files stay in memory after playback when using autoplay

---

## Requirements

- TikTok cookie and secuid ***or*** a pre-fetched music list in the cache
- ffmpeg installed on your system (for audio conversion)

---

## Troubleshooting
- If you get this error: `Unhandled exception: [json.exception.type_error.302] type must be string, but is object`, delete the config.json file

---

## Usage
If you want to use the provided example music list, download the file **musicList_0** and put it into the cache folder

1. Start the program
2. Follow the tutorial
3. Profit!

---

## How to get your cookie
1. Open the developer tools and go to the **Network** tab
2. Goto **[https://www.tiktok.com](https://www.tiktok.com)**
3. Search for **https://www.tiktok.com/api secuid** in the search bar top left
4. Right click any request and click **Copy as fetch (Node.js)**
5. Paste it into the input field in the bottom of the **Settings** page

---

## Features
### 1. Music Playback (Main Feature)
- Play TikTok saved sounds from the cache
- Handles conversion from many formats to `.mp3` on-the-fly if needed

### 2. Multi-Screen User Interface
- **Main Screen:** Music list, pagination, and playback controls
- **Settings Screen:** Change cookie, secuid and import them from devtools
- **Extras Screen:** Shows memory usage, fps amount and data management

### 3. Caching
- Downloads and caches sounds on disk to avoid repeated downloads
- Only the currently playing track is loaded in memory
- Reduces RAM usage

### 4. Playback Controls
- Autoplay next track
- Looping
- Volume control
- Pagination for long music lists

### 5. Data Management
- Delete cache
- Reset all data
- Edit JSON-based settings for cookie and secuid

---

## Optimization techniques
1. **Lazy Loading:** Only the currently playing audio file is loaded
2. **Caching:** Downloaded TikTok music and music lists are saved to disk
3. **Streamed Audio Playback:** Reduces RAM usage by streaming instead of fully loading
4. **Single Audio Instance:** Only one `ma_sound` instance exists
