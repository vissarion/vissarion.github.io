# -*- encoding: utf-8 -*-
# stub: al_charts 1.0.1 ruby lib

Gem::Specification.new do |s|
  s.name = "al_charts".freeze
  s.version = "1.0.1".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "allowed_push_host" => "https://rubygems.org", "homepage_uri" => "https://github.com/al-org-dev/al-charts", "source_code_uri" => "https://github.com/al-org-dev/al-charts" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["al-org".freeze]
  s.date = "1980-01-02"
  s.description = "Jekyll plugin extracted from al-folio that ships chart assets and conditionally renders Mermaid, ECharts, Plotly, Vega-Lite, Leaflet, and diff2html scripts.".freeze
  s.email = ["dev@al-org.dev".freeze]
  s.homepage = "https://github.com/al-org-dev/al-charts".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 2.7".freeze)
  s.rubygems_version = "4.0.10".freeze
  s.summary = "Chart and diagram asset pipeline for al-folio Jekyll sites".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<jekyll>.freeze, [">= 3.9".freeze, "< 5.0".freeze])
  s.add_runtime_dependency(%q<liquid>.freeze, [">= 4.0".freeze, "< 6.0".freeze])
  s.add_development_dependency(%q<bundler>.freeze, [">= 2.0".freeze, "< 5.0".freeze])
  s.add_development_dependency(%q<rake>.freeze, ["~> 13.0".freeze])
end
