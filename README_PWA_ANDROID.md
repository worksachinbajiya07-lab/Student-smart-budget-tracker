# Student Smart Budget Tracker — Installable PWA (Android Chrome)

## What's included
- `index.html` — responsive website UI
- `manifest.json` — PWA name, app icon and standalone launch settings
- `service-worker.js` — caches the app shell for offline launch
- `icons/icon-192.png` and `icons/icon-512.png` — app icons
- `backend.cpp` — C++ HTTP API and SQLite database
- `Dockerfile`, `render.yaml` — optional Render backend deployment config

## Important architecture note
The PWA frontend can be hosted for free as a static site. But if you want the app to save expenses from phones, the C++ backend must be deployed on a public HTTPS endpoint too.

The included Render setup is for a demo. Render's free web services use an ephemeral filesystem, so SQLite data can disappear on restart/redeploy/spin-down. Do not use this free SQLite deployment for important or permanent records. For durable online data, use a paid persistent disk for SQLite or change the backend to a managed database such as PostgreSQL. Render's free Postgres offering has its own expiry/limitations, so review current provider terms before relying on it.

## A. Test locally on your Mac

1. Install dependencies:
   ```bash
   brew install cpp-httplib nlohmann-json sqlite
   ```
2. Build:
   ```bash
   clang++ backend.cpp -std=c++17 -o backend \
     -I"$(brew --prefix cpp-httplib)/include" \
     -I"$(brew --prefix nlohmann-json)/include" \
     -lsqlite3 -pthread
   ```
3. Terminal 1:
   ```bash
   ./backend
   ```
4. Terminal 2, from this folder:
   ```bash
   python3 -m http.server 8000
   ```
5. On the Mac, open `http://localhost:8000`.
   Note: install prompts generally require HTTPS, or localhost during local development.

## B. Publish the PWA frontend free with GitHub Pages

1. Sign in or create an account at https://github.com
2. Create a **public** repository, e.g. `student-smart-budget-tracker`.
3. Upload the files from this folder, keeping `manifest.json`, `service-worker.js`, and the `icons` folder at the repository root.
4. In repository Settings → Pages, choose deploy from the `main` branch and `/ (root)`, then save.
5. Wait for GitHub Pages to publish the HTTPS URL, usually shaped like:
   `https://YOUR-USERNAME.github.io/student-smart-budget-tracker/`

GitHub Pages hosts static files only; it does not run your C++ backend.

## C. Connect the website to an online backend

1. Deploy the C++ backend to a hosting provider that supports Docker/C++ and HTTPS.
2. If using the included Render config, connect the GitHub repository to Render and create the web service from `render.yaml` (or select the Docker runtime).
3. Copy the backend's HTTPS URL, for example `https://your-api.onrender.com`.
4. In `index.html`, replace:
   ```js
   const API = 'http://localhost:8080/api';
   ```
   with:
   ```js
   const API = 'https://YOUR-BACKEND-URL/api';
   ```
5. Commit the change and wait for GitHub Pages to redeploy.
6. Open the deployed backend URL plus `/api/health` to check it responds with JSON.

**Free Render limitation:** the free web service filesystem is ephemeral. SQLite records may be lost when the service restarts or spins down. The service can also take around a minute to wake after inactivity. This is acceptable for a demo, not reliable permanent storage.

Also, do not leave the API set to `localhost` in the published website: on a phone, `localhost` means the phone itself, not your Mac/server.

## D. Install on Android Chrome

1. Open the deployed HTTPS GitHub Pages URL in Chrome on Android.
2. Wait for the page to load.
3. Open Chrome's ⋮ menu.
4. Tap **Install app** or **Add to Home screen** (the wording can vary).
5. Confirm. The app icon appears on the Home Screen/app launcher.

If Chrome does not show the install option, confirm that the website is served over HTTPS and `manifest.json`, icons, and service worker load without errors.

## Offline behavior
The service worker caches the app shell, so the interface can reopen offline after the first successful visit. The backend API requires an internet connection; adding, deleting, or refreshing expenses will not work while offline.

## Security note
This beginner demo has no user accounts or per-user isolation. Do not put sensitive personal financial data on a public shared backend until authentication and data separation are added.
