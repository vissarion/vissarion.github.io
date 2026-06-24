---
layout: page
title: Talks
permalink: /talks/
description: Conference and workshop presentations.
nav: true
nav_order: 3
---

{% assign talks_by_year = site.data.talks | group_by: "year" %}
{% assign sorted_years = talks_by_year | sort: "name" | reverse %}

<!-- Type filter buttons -->
<div class="talk-filters" style="margin-bottom: 1.5rem;">
  <button class="talk-filter-btn active" data-type="all" style="margin-right: 0.4rem; margin-bottom: 0.3rem; padding: 0.25rem 0.8rem; border: 1px solid var(--global-divider-color); border-radius: 4px; background: var(--global-theme-color); color: #fff; cursor: pointer; font-size: 0.85rem;">All</button>
  <button class="talk-filter-btn" data-type="conference" style="margin-right: 0.4rem; margin-bottom: 0.3rem; padding: 0.25rem 0.8rem; border: 1px solid var(--global-divider-color); border-radius: 4px; background: transparent; color: var(--global-text-color); cursor: pointer; font-size: 0.85rem;">Conference</button>
  <button class="talk-filter-btn" data-type="invited" style="margin-right: 0.4rem; margin-bottom: 0.3rem; padding: 0.25rem 0.8rem; border: 1px solid var(--global-divider-color); border-radius: 4px; background: transparent; color: var(--global-text-color); cursor: pointer; font-size: 0.85rem;">Invited</button>
  <button class="talk-filter-btn" data-type="workshop" style="margin-right: 0.4rem; margin-bottom: 0.3rem; padding: 0.25rem 0.8rem; border: 1px solid var(--global-divider-color); border-radius: 4px; background: transparent; color: var(--global-text-color); cursor: pointer; font-size: 0.85rem;">Workshop</button>
  <button class="talk-filter-btn" data-type="lightning" style="margin-right: 0.4rem; margin-bottom: 0.3rem; padding: 0.25rem 0.8rem; border: 1px solid var(--global-divider-color); border-radius: 4px; background: transparent; color: var(--global-text-color); cursor: pointer; font-size: 0.85rem;">Lightning</button>
  <button class="talk-filter-btn" data-type="meetup" style="margin-right: 0.4rem; margin-bottom: 0.3rem; padding: 0.25rem 0.8rem; border: 1px solid var(--global-divider-color); border-radius: 4px; background: transparent; color: var(--global-text-color); cursor: pointer; font-size: 0.85rem;">Meetup</button>
</div>

<!-- Text search (same as publications) -->
<script src="/assets/js/bibsearch.js?v={{ site.time | date: '%s' }}" type="module"></script>
<p><input type="text" id="bibsearch" spellcheck="false" autocomplete="off" class="search bibsearch-form-input" placeholder="Type to filter"></p>

<div class="publications">

{% for year in sorted_years %}

<h2 class="bibliography">{{ year.name }}</h2>

<ol class="bibliography">

{% for talk in year.items %}

<li data-type="{{ talk.type }}">
  <div class="row">

    <div class="col col-sm-2 abbr">
      <abbr class="badge rounded w-100" style="background-color:{{ talk.color }}">
        {{ talk.event }}
      </abbr>
    </div>

    <div id="{{ talk.title | slugify }}" class="col-sm-8">
      <div class="title">{{ talk.title }}</div>

      <div class="links">
        {% if talk.slides %}
          <a class="btn btn-sm z-depth-0" role="button" href="{{ talk.slides | prepend: '/assets/pdf/' | relative_url }}">
            <i class="fa-solid fa-file-pdf"></i> PDF
          </a>
        {% endif %}
        {% if talk.video %}
          <a class="btn btn-sm z-depth-0" role="button" href="{{ talk.video }}" target="_blank" rel="external nofollow noopener">
            <i class="fa-solid fa-video"></i> Video
          </a>
        {% endif %}
      </div>
    </div>

  </div>
</li>

{% endfor %}

</ol>

{% endfor %}

</div>

<script>
  // Type filter for talks
  document.addEventListener('DOMContentLoaded', function() {
    const buttons = document.querySelectorAll('.talk-filter-btn');
    buttons.forEach(function(btn) {
      btn.addEventListener('click', function() {
        const type = this.getAttribute('data-type');
        // Update active button
        buttons.forEach(function(b) {
          b.style.background = 'transparent';
          b.style.color = getComputedStyle(document.documentElement).getPropertyValue('--global-text-color').trim();
        });
        this.style.background = getComputedStyle(document.documentElement).getPropertyValue('--global-theme-color').trim();
        this.style.color = '#fff';

        // Show/hide entries
        document.querySelectorAll('.bibliography > li').forEach(function(li) {
          if (type === 'all' || li.getAttribute('data-type') === type) {
            li.style.display = '';
          } else {
            li.style.display = 'none';
          }
        });

        // Show/hide year headers
        document.querySelectorAll('h2.bibliography').forEach(function(h2) {
          var ol = h2.nextElementSibling;
          if (ol && ol.tagName === 'OL') {
            var visible = ol.querySelectorAll('li[style*="display: none"], li[style*="display:none"]').length < ol.querySelectorAll('li').length;
            h2.style.display = visible ? '' : 'none';
          }
        });
      });
    });
  });
</script>
