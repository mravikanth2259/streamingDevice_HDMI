/**
 * Streaming Device UI Prototype - App launcher with river-style scroll
 * Controls: Left/Right arrows, Enter to select, S or Settings to open panel
 */

let LAYOUT = null;
let focusIndex = 0;
let panelOpen = false;
let railEl = null;
let tooltipEl = null;

const ICON_SIZE = 170;
const ICON_GAP = 28;

async function loadLayout() {
  try {
    const res = await fetch("data/layout.json");
    if (res.ok) LAYOUT = await res.json();
  } catch (e) {
    console.warn("Could not load layout.json (use a local server). Using defaults.");
  }
  if (!LAYOUT) {
    LAYOUT = {
      initialFocusIndex: 0,
      icons: [
        { id: "netflix", label: "Netflix", asset: "assets/icons/netflix.svg" },
        { id: "prime", label: "Prime Video", asset: "assets/icons/prime_video.svg" },
        { id: "hulu", label: "Hulu", asset: "assets/icons/hulu.svg" },
        { id: "disney", label: "Disney+", asset: "assets/icons/disney_plus.svg" },
        { id: "youtube", label: "YouTube", asset: "assets/icons/youtube.svg" },
        { id: "settings", label: "Settings", asset: "assets/icons/settings.svg", action: "open_settings" }
      ],
      networkIcons: [
        { id: "wifi", label: "Wi-Fi", status: "connected" },
        { id: "bluetooth", label: "Bluetooth", status: "disconnected" }
      ]
    };
  }
}

function getIconCount() {
  return LAYOUT?.icons?.length ?? 0;
}

function getFocusedIcon() {
  const icons = LAYOUT?.icons ?? [];
  return icons[focusIndex] ?? null;
}

function renderRail() {
  railEl = document.getElementById("iconRail");
  railEl.innerHTML = "";

  const icons = LAYOUT?.icons ?? [];
  icons.forEach((ico, idx) => {
    const item = document.createElement("div");
    item.className = "icon-rail-item";
    item.setAttribute("role", "listitem");

    const btn = document.createElement("button");
    btn.className = "icon-btn" + (idx === focusIndex ? " focused" : "");
    btn.setAttribute("aria-label", ico.label);
    btn.setAttribute("data-id", ico.id);
    btn.setAttribute("data-idx", idx);

    const img = document.createElement("img");
    img.src = ico.asset;
    img.alt = "";
    img.loading = "eager";
    btn.appendChild(img);

    const label = document.createElement("span");
    label.className = "icon-label";
    label.textContent = ico.label;
    btn.appendChild(label);

    btn.addEventListener("click", () => {
      focusIndex = idx;
      renderRail();
      handleSelect();
    });

    item.appendChild(btn);
    railEl.appendChild(item);
  });

  updateScrollOffset();
  updateTooltip();
}

function updateScrollOffset() {
  if (!railEl) return;

  const icons = LAYOUT?.icons ?? [];
  const visibleCount = Math.floor(window.innerWidth / (ICON_SIZE + ICON_GAP)) || 4;
  const totalWidth = icons.length * (ICON_SIZE + ICON_GAP) - ICON_GAP;
  const railWidth = railEl.offsetWidth || totalWidth;
  const maxOffset = Math.max(0, totalWidth - railWidth + 80);

  let offset = focusIndex * (ICON_SIZE + ICON_GAP) - railWidth / 2 + ICON_SIZE / 2 + ICON_GAP / 2;
  offset = Math.max(0, Math.min(offset, maxOffset));

  railEl.style.transform = `translateX(-${offset}px)`;
}

function updateTooltip() {
  tooltipEl = document.getElementById("tooltip");
  const icon = getFocusedIcon();
  if (!icon) {
    tooltipEl.classList.remove("visible");
    tooltipEl.setAttribute("aria-hidden", "true");
    return;
  }
  tooltipEl.textContent = icon.label;
  tooltipEl.classList.add("visible");
  tooltipEl.setAttribute("aria-hidden", "false");
}

function handleSelect() {
  const icon = getFocusedIcon();
  if (!icon) return;

  if (icon.action === "open_settings" || icon.id === "settings") {
    openSettings();
  } else {
    console.log("Launching app:", icon.id);
  }
}

function openSettings() {
  const modal = document.getElementById("settingsModal");
  modal.setAttribute("aria-hidden", "false");
  panelOpen = true;
}

function closeSettings() {
  const modal = document.getElementById("settingsModal");
  modal.setAttribute("aria-hidden", "true");
  panelOpen = false;
}

function onKey(e) {
  if (panelOpen) {
    if (e.key === "Escape") {
      closeSettings();
      e.preventDefault();
    } else if (e.key === "Enter") {
      const active = document.activeElement;
      if (active?.id === "closeSettings") {
        closeSettings();
        e.preventDefault();
      }
    }
    return;
  }

  const count = getIconCount();
  if (count === 0) return;

  if (e.key === "ArrowRight") {
    focusIndex = (focusIndex + 1) % count;
    renderRail();
    e.preventDefault();
  } else if (e.key === "ArrowLeft") {
    focusIndex = (focusIndex - 1 + count) % count;
    renderRail();
    e.preventDefault();
  } else if (e.key === "Enter") {
    handleSelect();
    e.preventDefault();
  } else if (e.key.toLowerCase() === "s") {
    openSettings();
    e.preventDefault();
  }
}

function setupNetworkIcons() {
  tooltipEl = document.getElementById("tooltip");
  const network = LAYOUT?.networkIcons ?? [];
  const wifiEl = document.getElementById("wifiIcon");
  const btEl = document.getElementById("bluetoothIcon");

  const wifiStatus = network.find(n => n.id === "wifi")?.status ?? "connected";
  const btStatus = network.find(n => n.id === "bluetooth")?.status ?? "disconnected";

  if (wifiEl) {
    wifiEl.setAttribute("aria-label", `Wi-Fi: ${wifiStatus === "connected" ? "Connected" : "Disconnected"}`);
    wifiEl.setAttribute("title", `Wi-Fi: ${wifiStatus === "connected" ? "Connected" : "Disconnected"}`);
    const badge = wifiEl.querySelector(".status-badge");
    if (badge) badge.className = "status-badge " + (wifiStatus === "connected" ? "connected" : "disconnected");
    wifiEl.addEventListener("focus", () => {
      if (tooltipEl) {
        tooltipEl.textContent = "Wi-Fi: " + (wifiStatus === "connected" ? "Connected" : "Disconnected");
        tooltipEl.classList.add("visible");
      }
    });
    wifiEl.addEventListener("blur", () => { if (tooltipEl) tooltipEl.classList.remove("visible"); });
  }

  if (btEl) {
    btEl.setAttribute("aria-label", `Bluetooth: ${btStatus === "connected" ? "Connected" : "Disconnected"}`);
    btEl.setAttribute("title", `Bluetooth: ${btStatus === "connected" ? "Connected" : "Disconnected"}`);
    const badge = btEl.querySelector(".status-badge");
    if (badge) badge.className = "status-badge " + (btStatus === "connected" ? "connected" : "disconnected");
    btEl.addEventListener("focus", () => {
      if (tooltipEl) {
        tooltipEl.textContent = "Bluetooth: " + (btStatus === "connected" ? "Connected" : "Disconnected");
        tooltipEl.classList.add("visible");
      }
    });
    btEl.addEventListener("blur", () => { if (tooltipEl) tooltipEl.classList.remove("visible"); });
  }
}

function setupModal() {
  document.getElementById("closeSettings").addEventListener("click", closeSettings);
  document.getElementById("modalBackdrop").addEventListener("click", closeSettings);
}

async function init() {
  await loadLayout();
  focusIndex = LAYOUT?.initialFocusIndex ?? 0;
  renderRail();
  setupNetworkIcons();
  setupModal();
  document.addEventListener("keydown", onKey);

  window.addEventListener("resize", () => {
    updateScrollOffset();
  });
}

init();
